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

#include <avr/io.h>
#include <
#define F_CPU 8000000UL  // 8 MHz
#include <util/delay.h>
#include <stdint.h>
#define DD_MOSI DDB3
#define DD_SCK  DDB5
#define DD_SS0   DDC0
#define DD_SS1   DDC1
#define DDR_SPI DDRB

union u_intchar {
uint16_t i;
uint8_t c[2];
}

void SPI_MasterInit(void)
{
  /* Set MOSI and SCK output, all others input */
  DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
  /*Set SC pins to output*/
  DDRC = (1<<DDC0)|(1<<DDC1);
  DDRC = 1;
  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
  // Double SPI Speed
  SPSR = (1<<SPI2X)
}

uint8_t readctrlreg(uint8_t addr)
{

  uint8_t a;
  DDRC &= 0xFE; //pull CS low
  SPDR=addr; //set LS 3 bits to address of ENC register
  while(!(SPSR & (1<<SPIF))) ; //wait until end of transmission
  if (addr!=/*ETH control address range*/){
    a=SPDR;
    while(!(SPSR & (1<<SPIF))) ;
  }
    a=SPDR;
    DDRC |= 1; //release CS
    return a;
}

void writectrlreg(uint8_t addr, uint8_t data)
{
  DDRC &= 0xFE; //pull CS low
  SPDR = 0x40 + addr;
  while(!(SPSR & (1<<SPIF))) ;
  SPDR = data;
  while(!(SPSR & (1<<SPIF))) ;
  DDRC |= 1; //release CS
}
void readbuffer(uint8_t* outbuf, int nbytes)
{
  DDRC &= 0xFE;
  SPDR = 0x3A;
  for (i=0;i<bytes;i++){
    while(!(SPSR & (1<<SPIF))) ;
    outbuf[i]=SPDR;
  }
  DDRC |= 1; 
}

void writebuffer(uint8_t* inbuf, int nbytes)
{
  DDRC &= 0xFE;
  SPDR = 0x7A;
  for (i=0;i<bytes;i++){  //ENC28j60 automatically increments through buffer with each read
    while(!(SPSR & (1<<SPIF))) ;
    SPDR=inbuf[i];
  }
  DDRC |= 1; 
}

void bitfieldset(uint8_t addr, uint8_t bitmask) //bitwise OR
{
  DDRC &= 0xFE;
  SPDR = 0x80 + addr;
  while(!(SPSR & (1<<SPIF))) ;
  SPDR = bitmask;
  while(!(SPSR & (1<<SPIF))) ;
  DDRC |= 1;
}

void bitfieldclear(uint8_t addr, uint8_t bitmask) //bitwise NOT AND, ie. 1 resets bit, 0 leaves bit as it was.
{
  DDRC &= 0xFE;
  SPDR = 0xA0 + addr;
  while(!(SPSR & (1<<SPIF))) ;
  SPDR = bitmask;
  while(!(SPSR & (1<<SPIF))) ;
  DDRC |= 1;
}

uint16_t readphybuffer(uint8_t addr)
{
  writectrlreg(MIREGADR, addr);
  bitfieldset(MICMD,(1<<MIIRD));
  while(readctrlreg(MISAT,BUSY))
    yield();
  bitfieldclear(MICMD,(1<<MIIRD));
  return readctrlreg(MIRDL) + readctrlreg(MIRDH);
}
void writephybuffer(uint8_t addr, u_intchar data)
{
  writectrlreg(MIREGADR, addr);
  writectrlreg(MIWRL, data.c[0]);
  writectrlreg(MIWRH, data.c[1]);
  while(readctrlreg(MISAT,BUSY))
    yield();
}

void enc_init()
{
writectrlreg(MACON1, (1<<MARXEN)|(1<<TXPAUS)|(1<<RXPAUS));
writectrlreg(MACCON3, (1<<PADCFG0)|(1<<TXCRCEN)|(1<<FULDPX));
writectrlreg(ERXFCON, 0x80);
writectrlreg(ECON1, (1<<RXEN);
writectrlreg(ECON2, (1<<AUTOINC);
}
