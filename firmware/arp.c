#include <flags.h>
#include <types.h>

typedef struct {
  mac_address mac;
  IP_address ip;
} arp_entry;
arp_entry arp_hash[16];
arp_entry arp_hash_bucket[4];
uint8_t reverse_arp_hash[16];
arp_add(IP_address ip, mac_address mac)
{
  uint8_t index = h(ip);
  arp_hash[index].mac=mac;
  arp_hash[index].ip=ip;
  reverse_arp_hash[h(mac)] = index;
}

arp_lookup(IP_address ip);
{
  uint8_t index= h(ip);
  if (arp_hash[index].ip=0)
    return 0;
  else if (arp_hash[index].ip=ip)
    return arp_hash[index].mac;
  else


