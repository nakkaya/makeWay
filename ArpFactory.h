#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <vector>

//htons linux
#include <sys/types.h>
#include <netinet/in.h>

#include <pcap.h>
#include "Host.h"
#include "network.h"

class ArpFactory{
private:
  struct arp_hdr arp_src;
  struct ether_hdr eth_src;
  uint16_t opcode;  
  char *packet;

public:
  void setArpOpCode( uint16_t oc );
  void setArpIP( uint8_t src_ip[4] , uint8_t tgt_ip[4] );
  void setArpMAC( uint8_t src_mac[6] , uint8_t tgt_mac[6] );
  void setEthMAC( uint8_t src_mac[6] , uint8_t tgt_mac[6] );
  char* create();
};
