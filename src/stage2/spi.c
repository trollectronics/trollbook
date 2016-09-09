#include <stdint.h>
#include "spi.h"
#include "delay.h"

void spi_select_slave(int slave) {
	while(SPI_REG_STATUS & 0x1);
	SPI_REG_SS = slave;
}

uint8_t spi_send_recv(uint8_t dat) {
	while(SPI_REG_STATUS & 0x1);
	SPI_REG_DATA = dat;
	__asm__ __volatile__ ("nop\n"); //WTF WHY DO I NEED YOU?
	while(SPI_REG_STATUS & 0x1);
	return SPI_REG_DATA;
}

void spi_set_clockdiv(uint16_t clkdiv) {
	SPI_REG_STATUS = ((uint32_t) clkdiv) << 16;
}
