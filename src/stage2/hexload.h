#ifndef _LOADHEX_H_
#define _LOADHEX_H_
#include <stdint.h>

int hexload_verify_byte(volatile uint8_t *addr, uint8_t data, void *arg);
int hexload_write_byte(volatile uint8_t *addr, uint8_t data, void *arg);

int hexload(uint8_t (* fetch_byte)(int), int fd, int (data_byte)(volatile uint8_t *addr, uint8_t data, void *arg), void *data_byte_arg, void **addr_out);
#endif
