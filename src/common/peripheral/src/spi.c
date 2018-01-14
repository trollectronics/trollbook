#include <stdint.h>
#include "spi.h"
#include "delay.h"
#include "util.h"

void spi_select_slave(int slave) {
	//uint32_t count = 0;
	nop();nop();
	while(SPI_REG_STATUS & 0x1) {
		nop();nop();//printf("%u\n", count++);
	}
	nop();nop();
	SPI_REG_SS = slave;
	nop();
}

uint8_t spi_send_recv(uint8_t dat) {
	//uint32_t count = 0;
	uint8_t ret;
	nop();nop();
	while(SPI_REG_STATUS & 0x1) {
		nop();nop();
	}
	nop();nop();
	SPI_REG_DATA = dat;
	nop();nop(); //Nops force pipeline synchronization, needed when reading from IO devices
	while(SPI_REG_STATUS & 0x1) {
		nop();nop();//printf("%u\n", count++);
	}
	nop();nop();
	ret = SPI_REG_DATA;
	nop();nop();
	return ret;
}

void spi_set_clockdiv(uint16_t clkdiv) {
	nop();
	SPI_REG_STATUS = ((uint32_t) clkdiv) << 16;
	nop();
}

uint16_t spi_get_clockdiv() {
	uint16_t ret;
	nop();
	ret = ((SPI_REG_STATUS) >> 16);
	nop();
	return ret;
}
