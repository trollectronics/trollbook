#include <stdint.h>
#include "uart.h"
#include "spi.h"
#include "delay.h"
#include "input.h"

InputButtons input_poll_uart() {
	char c;
	InputButtons btn = {};
	c = uart_recv();
	
	switch(c) {
		case 'w':
			btn.up = 1;
			break;
		case 'a':
			btn.left = 1;
			break;
		case 's':
			btn.down = 1;
			break;
		case 'd':
			btn.right = 1;
			break;
		case ' ':
			btn.enter = 1;
			break;
		case 27:
			btn.back = 1;
			break;
	}
	
	return btn;
}

InputButtons input_poll_temp_spi() {
	uint8_t reg;
	uint16_t clockdiv;
	InputButtons btn = {};
	
	clockdiv = spi_get_clockdiv();
	spi_set_clockdiv(165);
	
	do {
		spi_select_slave(SPI_SLAVE_KBD);
		dumbdelay(1000);
		reg = spi_send_recv(0xFF);
		spi_select_slave(SPI_SLAVE_NONE);
		dumbdelay(1000);
	} while(reg != 0x3F);
	
	do {
		spi_select_slave(SPI_SLAVE_KBD);
		dumbdelay(1000);
		reg = spi_send_recv(0xFF);
		spi_select_slave(SPI_SLAVE_NONE);
		dumbdelay(1000);
	} while(reg == 0x3F);
	
	btn.left = !(reg & 0x1);
	btn.right = !(reg & 0x2);
	btn.up = !(reg & 0x4);
	btn.down = !(reg & 0x8);
	btn.enter = !(reg & 0x10);
	btn.back = !(reg & 0x20);
	
	spi_set_clockdiv(clockdiv);
	return btn;
}
