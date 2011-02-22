#include "ArpFactory.h"

void ArpFactory::setArpOpCode( uint16_t oc ){
    opcode = oc;
  }

  void ArpFactory::setArpIP( uint8_t src_ip[4] , uint8_t tgt_ip[4] ){
    for(int i = 0 ; i<4 ; i++ ){
      arp_src.src_ip[i] = src_ip[i];
      arp_src.tgt_ip[i] = tgt_ip[i];
    }
  }

  void ArpFactory::setArpMAC( uint8_t src_mac[6] , uint8_t tgt_mac[6] ){
    for(int i = 0 ; i<6 ; i++ ){
      arp_src.src_mac[i] = src_mac[i];
      arp_src.tgt_mac[i] = tgt_mac[i];
    }
  }

  void ArpFactory::setEthMAC( uint8_t src_mac[6] , uint8_t tgt_mac[6] ){
    for(int i = 0 ; i<6 ; i++ ){
      eth_src.h_source[i] = src_mac[i];
      eth_src.h_dest[i] = tgt_mac[i];
    }
  }

  char* ArpFactory::create(){
    packet = (char*)malloc(sizeof(struct ether_hdr) + sizeof(struct arp_hdr));

    struct ether_hdr eth_frame;
    struct arp_hdr arp_frame;
    
    memcpy(eth_frame.h_dest, eth_src.h_dest, 6);
    memcpy(eth_frame.h_source, eth_src.h_source, 6);

//     printMAC("eth_frame.h_dest" , eth_src.h_dest );
//     printMAC("eth_frame.h_source" , eth_src.h_source );
    
    eth_frame.h_proto = htons(ARP_PROTO);
    
    arp_frame.htype = htons(HW_ETHER);
    arp_frame.proto = htons(IP_PROTO);
    arp_frame.hsize = HW_SIZE;
    arp_frame.psize = PROTO_SIZE;
    arp_frame.opcode = htons(opcode);
    
    memcpy(arp_frame.src_mac, arp_src.src_mac, 6);
    memcpy(arp_frame.src_ip, arp_src.src_ip, 4);
    memcpy(arp_frame.tgt_mac, arp_src.tgt_mac, 6);
    memcpy(arp_frame.tgt_ip, arp_src.tgt_ip, 4);
    
//     printIP("arp_src.src_ip" , arp_src.src_ip );
//     printMAC("arp_src.src_mac" , arp_src.src_mac );

//     printIP("arp_src.tgt_ip" , arp_src.tgt_ip );
//     printMAC("arp_src.tgt_mac" , arp_src.tgt_mac );
    
    memcpy(packet, &eth_frame, sizeof(struct ether_hdr));
    memcpy(packet + sizeof(struct ether_hdr), &arp_frame, sizeof(struct arp_hdr));
    
    return packet;
  }
