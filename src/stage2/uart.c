#include <stdint.h>
#include "uart.h"

void uart_send(uint8_t dat) {
	while(!(UART_REG_STATUS & 0x1));
	UART_REG_DATA = dat;
}

uint8_t uart_recv() {
	while(!(UART_REG_STATUS & 0x2));
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