#include <stdint.h>
#include "util.h"
#include "uart.h"

void uart_send(uint8_t dat) {
	nop();
	while(!(UART_REG_STATUS & 0x1));
	nop();
	UART_REG_DATA = dat;
	nop();
}

uint8_t uart_flush() {
	nop();
	return UART_REG_DATA;
}

uint8_t uart_recv() {
	nop();
	while(!(UART_REG_STATUS & 0x2));
	nop();
	return UART_REG_DATA;
}

void uart_send_hex(uint8_t h) {
	char c;
	uart_send('0');
	uart_send('x');
	c = (h >> 4) + '0';
	if(c > '9')
		c += 'A' - '0';
	uart_send(c);
	
	c = (h & 0xF) + '0';
	if(c > '9')
		c += 'A' - '0';
	uart_send(c);
}

void uart_send_string(char *s) {
	char c;
	while((c = *s++))
		uart_send((uint8_t) c);
}

void uart_putc_convnl(char c) {
	if(c == '\n')
		uart_send('\r');
	uart_send((uint8_t) c);
}

void uart_puts_convnl(const char *s) {
	while(*s)
		uart_putc_convnl(*s++);
}
