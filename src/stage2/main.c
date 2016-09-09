#include <stdint.h>
#include "uart.h"
#include "spi.h"
#include "printf.h"
#include "boot_term.h"

int main() {
	uint8_t b[2];
	
	term_init();
	
	unsigned int i, col = 0;
	for(;;) {
		for(i = 0; i < 203; i++)
			term_puts("Hello, world ", (col++) % 256);
		
		term_set_pos(0, 0);
	}
	
	spi_set_clockdiv(165);
	
	spi_select_slave(1);
	spi_send_recv(0x90);
	spi_send_recv(0x0);
	spi_send_recv(0x0);
	spi_send_recv(0x0);
	b[0] = spi_send_recv(0xFF);
	b[1] = spi_send_recv(0xFF);
	
	if(b[0] == 0x1 && b[1] == 0x12)
		printf("OK\n");
	else
		printf("Fail\n");
	
	printf("0x%X 0x%X\n", b[0], b[1]);
	
	
	for(;;);
		
	return 0;
}


