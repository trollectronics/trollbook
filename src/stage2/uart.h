#ifndef _UART_H_
#define _UART_H_
#include <stdint.h>
#include "peripheral.h"

#define UART_REG_DATA *((volatile uint32_t *) (PERIPHERAL_UART_BASE + 0x0))
#define UART_REG_STATUS *((volatile uint32_t *) (PERIPHERAL_UART_BASE + 0x4))

void uart_send(uint8_t dat);
uint8_t uart_flush();
uint8_t uart_recv();
void uart_send_hex(uint8_t h);
void uart_send_string(char *s);

void uart_putc_convnl(char c);
void uart_puts_convnl(const char *s);

#endif
