// Copyright 2011 Nurullah Akkaya

// makeWay is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.

// makeWay is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with makeWay. If not, see http://www.gnu.org/licenses/.

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
