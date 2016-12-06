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
		p1[n] = p2[n];
		n--;
	}
	
	return NULL;
}
