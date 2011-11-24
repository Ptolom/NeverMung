#include <flags.h>
#include <types.h>

typedef struct {
  mac_address mac;
  IP_address;
} arp_entry;
arp_entry arp_hash[16];
uint8_t reverse_arp_hash[16];

arp_lookup(IP_address;
