#include <stdio.h>

#define PING_IMPLEMENTATION
#include "ping.h"



int main()
{
  int8_t *str, *raw;
  icmp_t *pkt;

  str = (int8_t*)malloc(13);
  if(!str) exit(1);

  memset(str, 0, 13);
  memcpy(str, "Hello, World", 12);

  pkt = mkicmp(echo, str, 13);
  if(!pkt) {printf("Pkt error: 19\n"); exit(1);}

  showicmp(pkt);

  //s_size = sizeof(rawicmp_t) + pkt->size;
  raw = evalicmp(pkt);
  if(!raw) {printf("Raw pkt error: 24\n"); exit(1);}

  printf("raw info:\t %x\n", raw);
  //printf("raw info is %hhn\n", raw);
  free(pkt->data);
  free(pkt);
  return 0;
}
