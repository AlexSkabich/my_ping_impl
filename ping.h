#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define compact __attribute__ ((packed))


typedef enum
  {
    echo,
    echoreply,
    unassigned
  } e_type;

struct icmp_t
{
  e_type type;
  int8_t data[];

} compact;

struct rawicmp_t
{
  int8_t type;
  int8_t code;
  int16_t checksum;
  int8_t data[];
} compact;

typedef struct rawicmp_t rawicmp_t;
typedef struct icmp_t icmp_t;

rawicmp_t *mkicmp(int8_t type, int8_t code, int8_t *data, int16_t size);


#if defined(PING_IMPLEMENTATION)


//struct rawicmp_t rawwy1 [[gnu::aligned(1)]];

rawicmp_t *mkicmp(int8_t type, int8_t code, int8_t *data, int16_t size)
{
  if(!data || !size) return (struct rawicmp_t*)0;

  int16_t n;
  struct rawicmp_t *p;
  n = sizeof(icmp_t) + size;
  p = (rawicmp_t*)malloc(n);
  if(!p) exit(1);
  memset(p,0,n);
  p->type = type;
  p->code = code;
  memcpy(p->data, data, n);
  //p->checksum = checksum();
  //TODO: implement checksum function
  return p;
}



#endif
