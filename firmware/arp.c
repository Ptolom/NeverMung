#include <flags.h>
#include <types.h>
#define ARP_HASH_SIZE 32

typedef struct {
  mac_address mac;
  IP_address ip;
} arp_entry;
arp_entry arp_hash[16];
arp_entry arp_hash_bucket[4];
uint8_t reverse_arp_hash[16];
hash_ip(IP_address ip)
{
  uint8_t keysize=log(2,ARP_HASH_SIZE); //move to init function and make static
  uint8_t shunt_1, shunt_2
  shunt_1=  (16-keysize)/2+keysize;
  shunt_2=  (16-keysize)/2;
  if (keysize%2 ==1)
    shunt_2++;

  uint8_t temp =ip.c[6]*ip.c[6];
  temp = temp << shunt_1

arp_add(IP_address ip, mac_address mac)
{
  uint8_t home, index, i;
  home = index = hash_ip(ip);
  for (i=0;(arp_hash[index].mac!=0)&&i<ARP_HASH_SIZE;i++){
    index=home + (i*i+i)/2;
  }
  if (i==ARP_HASH_SIZE){
    set(ARP_HASH_FULL);
  }
  arp_hash[index].mac=mac;
  arp_hash[index].ip=ip;
  reverse_arp_hash[hash_mac(mac)] = index;
}

arp_lookup(IP_address ip);
{
  uint8_t index= h(ip);
  if (arp_hash[index].ip=0)
    return 0;
  else if (arp_hash[index].ip=ip)
    return arp_hash[index].mac;
  else


