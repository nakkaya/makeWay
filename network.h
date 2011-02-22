// Copyright 2011 Nurullah Akkaya

// Mocha is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.

// Mocha is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with Mocha. If not, see http://www.gnu.org/licenses/.

// ARP protocol constants
#define ARP_PROTO     0x0806
#define IP_PROTO      0x0800

#define HW_ETHER      0x0001
#define HW_SIZE       0x06

#define PROTO_SIZE    0x04

// ARP opcode constants
#define ARP_REQUEST   0x0001
#define ARP_REPLY     0x0002
#define RARP_REQUEST  0x0003
#define RARP_REPLY    0x0004

// ethernet frame definition
struct ether_hdr{
  uint8_t h_dest[6];
  uint8_t h_source[6];
  uint16_t h_proto;
};

// arp frame definition
struct arp_hdr{
  uint16_t htype;
  uint16_t proto;
  uint8_t hsize;
  uint8_t psize;
  uint16_t opcode;
  uint8_t src_mac[6];
  uint8_t src_ip[4];
  uint8_t tgt_mac[6];
  uint8_t tgt_ip[4];
};

//struct to pass to pcap_loop
struct pcapLoopHandler{
  pcap_t *ph;
  time_t arpListenerRunningFor;
  vector<Host> targetList;
};
