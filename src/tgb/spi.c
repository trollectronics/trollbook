#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "protocol.h"

void spi_init() {
	DDRB |= (1 << 4); //MISO output
	DDRB &= ~(1 << 3); //MOSI input
	DDRB &= ~(1 << 2); //SS input
	DDRB &= ~(1 << 5); //SCK input
	
	(void) SPDR;
	SPCR = 0xC0;
}

void spi_deinit() {
	DDRB &= ~(1 << 4); //MISO input
	SPCR = 0x0;
}

void spi_send(uint8_t data) {
	SPDR = data;
}

uint8_t spi_recv() {
	return SPDR;
}

void spi_interrupt() {
	
}
