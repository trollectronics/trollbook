#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "sleep.h"

#define ABS(a) ((a) > 0 ? (a) : (-a))


void init_spi_slave() {
	USICR = (1 << USIWM0) | (1 << USICS1);
}

void do_transfer(uint8_t data) {
	USIDR = data;
	DDRA |= (1 << 5);
	USISR = (1 << USIOIF);
	while (!(USISR & (1 << USIOIF)));
	DDRA &= ~(1 << 5);

	return;
}


int main() {
	uint8_t keys = 0xFF;

	init_spi_slave();
	
	for(;;) {
		// TODO: Replace this with actual chip select pin //
		if (!(PINB & 0x4)) {
			do_transfer(keys);
			while (!(PINB & 0x4));
		}

		keys = PINA & 0xF;
		keys |= ((PINB & 0x3) << 4);
	}
	
	return 0;
}
