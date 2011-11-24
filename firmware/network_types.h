#include <types.h>

union IP_address{
uint64_t i;
uint8_t c[6];
}

union mac_address {
uint64_t i;
uint8_t c[6];
}


typedef struct {
  mac_address DA;
  mac_address SA;
  u_intchar Type;
} eth_packet;

typedef struct {
  uint8_t version;
  uint8_t IHL;
  uint16_t Type;
  uint16_t length;
  uint16_t ID;
  uint8_t fragment_flags;
  uint8_t TTL;
  uint8_t protocol;
  uint8_t checksum; //one's complement of one's complement sum. huh?
  IP_address source;
  IP_address dest;
} IP_Packet

