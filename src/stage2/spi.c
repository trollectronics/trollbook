#include <stdint.h>
#include "spi.h"

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