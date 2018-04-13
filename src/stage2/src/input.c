#include <stdint.h>
#include <stdbool.h>
#include "terminal.h"
#include "printf.h"
#include "uart.h"
#include "spi.h"
#include "delay.h"
#include "input.h"
#include "../../kbd/protocol.h"

static const char matrix_key[4][17] = {
	{'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '\'', 'B', 'T', 'A', 'U', 'C', },
	{'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', '.', 'R', 'G', '<', 'R', ']', },
	{'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'o', 'a', '\'', 'E', 'T', '$', 'D', '|', },
	{'S', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 'S', 'F', ' ', 'F', 'L', '[', },
};

static bool matrix[4][17];

//int matrix_cols[17] = {5, 1, 0, 2, 6, 7, 3, 4, 8, 14, 10, 13, 9, 15, 11, 12, 16};
//static const int matrix_cols[17] = {1, 3, 6, 7, 0, 4, 5, 8, 12, 10, 14, 15, 11, 9, 13, 16, 2};
static const int matrix_cols[17] = {2, 1, 3, 6, 7, 0, 4, 5, 8, 12, 10, 14, 15, 11, 9, 13, 16};
//static const int matrix_cols[17] = {0, 1, 2, 3, 4, 5, 6 ,7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

typedef struct InputMouseEvent InputMouseEvent;
struct InputMouseEvent {
	int16_t x;
	int16_t y;
	uint8_t buttons;
	int8_t wheel;
};

void input_test_keyboard(void *arg) {
	int i, j;
	uint8_t reg;
	InputMouseEvent mouse = {};
	spi_set_clockdiv(165);
	bool change;
	
	for(;;) {
		change = false;
		spi_select_slave(SPI_SLAVE_KBD);
		dumbdelay(1000);
		
		spi_send_recv(PROTOCOL_COMMAND_STATUS);
		dumbdelay(100);
		reg = spi_send_recv(0xFF);
		if(reg != 0xFF && reg & 0x1) {
			spi_send_recv(PROTOCOL_COMMAND_KEYBOARD_EVENT);
			
			while(dumbdelay(100), (reg = spi_send_recv(0xFF)) != 0xFF) {
				change = true;
				if(reg & 0x80) {
					matrix[reg & 0x3][matrix_cols[(reg & 0x7F) >> 2]] = false;
				} else {
					matrix[reg & 0x3][matrix_cols[reg >> 2]] = true;
				}
			}
		}
		
		if(reg != 0xFF && reg & 0x02) {
			spi_send_recv(PROTOCOL_COMMAND_MOUSE_EVENT);
			dumbdelay(100);
			spi_send_recv(0xFF);
			dumbdelay(100);
			
			mouse.x = ((uint16_t) spi_send_recv(PROTOCOL_COMMAND_MOUSE_EVENT)) << 8;
			dumbdelay(100);
			mouse.x |= spi_send_recv(PROTOCOL_COMMAND_MOUSE_EVENT);
			dumbdelay(100);
			mouse.y = ((uint16_t) spi_send_recv(PROTOCOL_COMMAND_MOUSE_EVENT)) << 8;
			dumbdelay(100);
			mouse.y |= spi_send_recv(PROTOCOL_COMMAND_MOUSE_EVENT);
			dumbdelay(100);
			
			spi_send_recv(0xFF);
			dumbdelay(100);
			
		}
		
		spi_select_slave(SPI_SLAVE_NONE);
		
		if(change) {
			terminal_clear();
			for(i = 0; i < 4; i++) {
				for(j = 0; j < 17; j++) {
					if(matrix[i][j])
						terminal_set_bg(TERMINAL_COLOR_GREEN);
					else
						terminal_set_bg(TERMINAL_COLOR_BLACK);
					
					printf("%c ", matrix_key[i][j]);
				}
				printf("\n");
			}
			printf("\n(%i, %i)\n", mouse.x, mouse.y);
		} else {
			dumbdelay(100000);
		}
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
	uint16_t clockdiv;
	
	clockdiv = spi_get_clockdiv();
	spi_set_clockdiv(165);
	do {
		spi_select_slave(SPI_SLAVE_KBD);
		dumbdelay(10000);
		
		spi_send_recv(PROTOCOL_COMMAND_STATUS);
		dumbdelay(100);
		reg = spi_send_recv(0xFF);
		if(reg != 0xFF && reg & 0x1) {
			spi_send_recv(PROTOCOL_COMMAND_KEYBOARD_EVENT);
			
			while(dumbdelay(10000), (reg = spi_send_recv(0xFF)) != 0xFF) {
				switch(reg) {
					case 0xB0:
						btn.up = 1;
						break;
					case 0xB3:
						btn.left = 1;
						break;
					case 0xB2:
						btn.down = 1;
						break;
					case 0xB1:
						btn.right = 1;
						break;
					case 0xA5:
						btn.enter = 1;
						break;
					case 0xA4:
						btn.back = 1;
						break;
				}
			}
		}
		spi_select_slave(SPI_SLAVE_NONE);
		dumbdelay(100000);
	} while(!(btn.up || btn.down || btn.left || btn.right || btn.enter || btn.back));
	
	spi_set_clockdiv(clockdiv);
	
	return btn;
}
