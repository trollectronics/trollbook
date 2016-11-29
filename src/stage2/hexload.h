#ifndef _LOADHEX_H_
#define _LOADHEX_H_
#include <stdint.h>

void *hexload(uint8_t (* fetch_byte)(int), int fd);
int hexload_validate(uint8_t (* fetch_byte)(int), int fd);
#endif
