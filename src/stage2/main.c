#include <stdint.h>

#define SPI_REG_DATA *((uint32_t *) 0x200000UL)
#define SPI_REG_STATUS *((uint32_t *) 0x200004UL)
#define SPI_REG_SS *((uint32_t *) 0x200008UL)

#define UART_REG_DATA *((uint32_t *) 0x100000UL)
#define UART_REG_STATUS *((uint32_t *) 0x100004UL)

void spi_select_slave(int slave) {
	while(SPI_REG_STATUS & 0x1);
	SPI_REG_SS = slave;
}

uint8_t spi_send_recv(uint8_t dat) {
	while(SPI_REG_STATUS & 0x1);
	SPI_REG_DATA = dat;
	while(SPI_REG_STATUS & 0x1);
	return SPI_REG_DATA;
}

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

int main() {
	uint8_t b[2];
	uart_recv();
	uart_recv();
	uart_recv();
	uart_recv();
	uart_recv();
	uart_recv();
	uart_recv();
	uart_send_string("OK get ready:\r\n");
	uart_recv();
	
	for(;;)
		spi_send_recv('a');
	
	spi_select_slave(2);
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
	
	for(;;)
		
	return 0;
}


