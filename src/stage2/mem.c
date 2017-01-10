#include <stdint.h>
#include <stddef.h>
#include "mem.h"

void *memset(void *pointer, int c, uint32_t n) {
	uint8_t *p1 = pointer;
	while(n) {
		p1[--n] = c;
	}
	return NULL;
}

void *memcpy(void *dest, void *src, uint32_t n) {
	uint8_t *p1 = dest, *p2 = src;
	while(n) {
		n--;
		p1[n] = p2[n];
	}
	
	return NULL;
}

char *strcpy(char *dest, char *src) {
	char *ret = dest;
	while(*src) {
		*dest++ = *src++;
	}
	
	return ret;
}