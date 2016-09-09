#include <stdint.h>
#include "uart.h"
#include "spi.h"
#include "boot_term.h"

int main() {
	//uint8_t b[2];
	
	term_init();
	
	unsigned int i, col = 0;
	for(;;) {
		for(i = 0; i < 203; i++)
			term_puts("Hello, world ", (col++) % 256);
		
		term_set_pos(0, 0);
	}
	
	/*uart_recv();
	uart_recv();
	uart_recv();
	uart_recv();
	uart_recv();
	uart_recv();
	uart_recv();
	uart_send_string("OK get ready:\r\n");
	uart_recv();
	
	spi_select_slave(1);
	spi_send_recv(0x90);
	spi_send_recv(0x0);
	spi_send_recv(0x0);
	spi_send_recv(0x0);
	b[0] = spi_send_recv(0xFF);
	b[1] = spi_send_recv(0xFF);
	
	if(b[0] == 0x1 && b[1] == 0x12)
		uart_send_string("OK\r\n");
	else
		uart_send_string("Fail\r\n");
	
	uart_send_hex(b[0]);
	uart_send(' ');
	uart_send_hex(b[1]);
	uart_send_string("\r\n");
	*/
	for(;;);
		
	return 0;
}


