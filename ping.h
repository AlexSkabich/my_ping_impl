#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef enum
  {
    echo,
    echoreply,
    unassigned
  } e_type;

struct icmp_t
{
  e_type type;
  int16_t size;
  int8_t *data;
} __attribute__ ((packed));

struct rawicmp_t
{
  int8_t type;
  int8_t code;
  int16_t checksum;
  int8_t data[];
} __attribute__ ((packed));

typedef struct rawicmp_t rawicmp_t;
typedef struct icmp_t icmp_t;

icmp_t *mkicmp(e_type type, const int8_t *data, int16_t size);
int8_t *evalicmp(icmp_t*);
void showicmp(icmp_t*);
int16_t mk_checksum(int16_t count, void* addr);





#if defined(PING_IMPLEMENTATION)


icmp_t *mkicmp(e_type type, const int8_t *data, int16_t size)
{
  if(!data || !size) return (icmp_t*)0;

  int16_t n;
  icmp_t *p;
  n = sizeof(icmp_t) + size;
  p = (icmp_t*)malloc(n);
  if(!p) exit(1);
  memset(p,0,n);
  p->type = type;
  p->size = size;
  //memcpy(p->data, data, n);
  p->data = data;
  return p;
}

void showicmp(icmp_t *pkt)
{
  if(!pkt) return;
  printf("type:\t %s\nsize:\t %d\npayload data:\t %s\n", (pkt->type == echo) ? "echo" : "echo reply", (int)pkt->size, (char*)pkt->data);
  return;
}

int8_t *evalicmp(icmp_t *pkt)
{
  if(!pkt || !pkt->data) {printf("No icmp package or empty data\n"); exit(1);}
  int8_t *p, *ret;
  int16_t checksum;
  rawicmp_t rawpkt, *rawptr;

  switch(pkt->type)
    {
    case echo:
      rawpkt.type = echo;
      rawpkt.code = 0;
      break;
    case echoreply:
      rawpkt.type = echoreply;
      rawpkt.code = 0;
      break;
    default: return (int8_t*)0;
    }
  rawpkt.checksum = 0;
  int16_t s_size  = sizeof(rawicmp_t) + pkt->size;
  if(!(s_size%2)) ++s_size;

  p = (int8_t*)malloc(s_size);
  ret = p;

  if(!p) { printf("Memory Allocation issue: 97\n"); exit(1);}
  memset(p, 0, s_size);

  memcpy(p, (int8_t *)&rawpkt, sizeof(rawicmp_t));
  p += sizeof(rawicmp_t);
  memcpy(p, pkt->data, pkt->size);

  checksum = mk_checksum(s_size, ret);
  rawptr = (rawicmp_t*)ret;
  rawptr->checksum = checksum;

  return ret;

}

int16_t mk_checksum(int16_t count, void* addr)
{
  int16_t *p;
  int32_t a;
  a = 0;
  for(p = (int16_t*)addr; count>0; count-=2, ++p)
    {
      a += *p;
    }

  return ~( ((a & 0xffff0000)>>16) + (a & 0xffff0000) );
}


#endif
