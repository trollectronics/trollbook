#include <stdint.h>
#include "spi.h"
#include "delay.h"
#include "util.h"

void spi_select_slave(int slave) {
	nop();
	while(SPI_REG_STATUS & 0x1);
	SPI_REG_SS = slave;
}

uint8_t spi_send_recv(uint8_t dat) {
	nop();
	while(SPI_REG_STATUS & 0x1);
	SPI_REG_DATA = dat;
	nop(); //Nops force pipeline synchronization, needed when reading from IO devices
	while(SPI_REG_STATUS & 0x1);
	nop();
	return SPI_REG_DATA;
}

void spi_set_clockdiv(uint16_t clkdiv) {
	SPI_REG_STATUS = ((uint32_t) clkdiv) << 16;
}

uint16_t spi_get_clockdiv() {
	return ((SPI_REG_STATUS) >> 16);
}
