/*
 *NeverMung - Copyright 2011 Hugh Coleman
 *
 *This file is part of NeverMung.
 *
 *NeverMung is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *NeverMung is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with NeverMung.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <flags.h>
#include <util/atomic.h>

int bufferedpackets;

checksum(uint16_t packet[],int l)
{
  uint32_t sum=0;
  int i;
  for(i=0;i<len;i++){
    sum+=packet[i];
    if(sum & 0x80000000)
      sum = (sum & 0xFFFF) + (sum >> 16);
  }
  return ~sum;
}

createpacket(uint8_t data, uint32_t address, char frag, int fragoffset, char protocol)
{
  uint8_t* packet;
  uint16_t* packetint=uint16_t*(packet);
  uint16_t* packet32int=uint32_t*(packet);
  char packet_ID=0;
  packetlength=sizeof(data)+IHL;
  packet=malloc(packetlength);
  packet[0]=4+IHL*16;
  packet[1]=0;
  packetint[2]=length;
  packetint[3]=packet_ID;
  packetint[4]=frag<<13;
  packetint[4]|=fragoffset;
  packetint[5]=0;
  packet[10]=0xFF;
  packet[11]=protocol;
  packet32int[3]=localIP;
  packet32int[4]=destIP;
  packetint[5]=checksum(packetint);
}

ETHmainloop()
{
  uint8_t packet[576];
  u_intchar eth_rx_write_pointer;
  u_intchar nextpointer;
  while(true)
  {
    eth_rx_read_pointer.c[1]=readctrlreg(ERDPTH);
    eth_rx_read_pointer.c[0]=readctrlreg(ERDPTL);
    packetsinbuffer = readctrlreg(EPKTCNT);
    readbuffer(nextpointer.c,2);
    length=nextpointer.i - eth_rx_read_pointer.i;
    if (length>MAX_PACKET_SIZE)
      icmp(FRAGMENT); //To implement
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
      readbuffer(packet, length);
      packet_processed=0;
    }
    process_eth(packet, length);
    yield(packet_processed);
  }
}
typedef struct {
  uint8_t DA[6];
  uint8_t SA[6];
  u_intchar Type;
  uint8_t data[MAX_PACKET_SIZE-18]
} eth_packet;
*eth_packet process_eth(uint8_t packet[MAX_PACKET_SIZE],int length) //process ethernet packets into separate fields and pass to IP
{
  eth_packet packet_fields;
  int i;
  for(i=0;i<6;i++)
    DA[i]=packet[i];
  for(i=0;i<6;i++)
    SA[i]=packet[i+6];
  for(i=0;i<2;i++)
    Type.c[i]=packet[i+12];
  for(i=0;i<length-18;i++) //until i=length of packet, less non data fields
    data[i]=packet[i+14];
  return &packet_fields;
}
