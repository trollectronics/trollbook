#include <stdint.h>
#include "uart.h"
#include "spi.h"
#include "delay.h"
#include "input.h"
#include "../tgb/protocol.h"

void input_test_keyboard(void *arg) {
	int i;
	uint8_t reg;
	StatusRegister status;
	terminal_clear();
	spi_set_clockdiv(165);
	printf("Testing keyboard\n");
	
	for(i= 0;;) {
		spi_select_slave(SPI_SLAVE_KBD);
		dumbdelay(1000);
		
		spi_send_recv(PROTOCOL_COMMAND_STATUS);
		reg = spi_send_recv(PROTOCOL_COMMAND_STATUS);
		if(reg != 0xFF && reg & 0x1) {
			spi_send_recv(PROTOCOL_COMMAND_KEYBOARD_EVENT);
			
			while((reg = spi_send_recv(PROTOCOL_COMMAND_KEYBOARD_EVENT)) != 0xFF) {
				if(i++ > 25) {
					i = 0;
					terminal_clear();
					printf("Testing keyboard\n");
				}
				printf("kbd ev %02x\n", (int) reg);
			}
		}
		spi_select_slave(SPI_SLAVE_NONE);
		dumbdelay(100000);
	}
}

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

InputButtons input_poll_keyboard(void *arg) {
	uint8_t reg;
	InputButtons btn = {};
	
	spi_set_clockdiv(165);
	do {
		spi_select_slave(SPI_SLAVE_KBD);
		dumbdelay(1000);
		
		spi_send_recv(PROTOCOL_COMMAND_STATUS);
		reg = spi_send_recv(PROTOCOL_COMMAND_STATUS);
		if(reg != 0xFF && reg & 0x1) {
			spi_send_recv(PROTOCOL_COMMAND_KEYBOARD_EVENT);
			
			while((reg = spi_send_recv(PROTOCOL_COMMAND_KEYBOARD_EVENT)) != 0xFF) {
					switch(reg) {
				case 0xAC:
					btn.up = 1;
					break;
				case 0xAF:
					btn.left = 1;
					break;
				case 0xAE:
					btn.down = 1;
					break;
				case 0xAD:
					btn.right = 1;
					break;
				case 0xA1:
					btn.enter = 1;
					break;
				case 0xA0:
					btn.back = 1;
					break;
			}
			}
		}
		spi_select_slave(SPI_SLAVE_NONE);
		dumbdelay(10000);
	} while(!(btn.up || btn.down || btn.left || btn.right || btn.enter || btn.back));
	
	return btn;
}
