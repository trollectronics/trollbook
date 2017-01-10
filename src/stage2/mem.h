#ifndef _MEM_H__
#define _MEM_H__

#include <stdint.h>

void *memset(void *pointer, int c, uint32_t n);
void *memcpy(void *dest, void *src, uint32_t n);
char *strcpy(char *dest, char *src);

#endif
