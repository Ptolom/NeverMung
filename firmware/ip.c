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

IPmainloop()
{
  uint8_t packet[576];
  u_intchar eth_rx_wrt_pt;
  while(true)
  {
    eth_rx_wrt_pt.c[1]=readctrlreg(ERXWRPTH)
    eth_rx_wrt_pt.c[0]=readctrlreg(ERXWRPTL)
    bufferlen= readctrlreg(EPKTCNT)
    nextpoint=
    readbuffer(
  }
}
