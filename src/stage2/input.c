#include <stdint.h>
#include "uart.h"
#include "input.h"

InputButtons input_poll() {
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
