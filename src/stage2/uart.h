#ifndef _UART_H_
#define _UART_H_
#include <stdint.h>

#define UART_REG_DATA *((uint32_t *) 0x100000UL)
#define UART_REG_STATUS *((uint32_t *) 0x100004UL)

void uart_send(uint8_t dat);
uint8_t uart_recv();
void uart_send_hex(uint8_t h);
void uart_send_string(char *s);

#endif
