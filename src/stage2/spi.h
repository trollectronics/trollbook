#ifndef _SPI_H_
#define _SPI_H_
#include <stdint.h>

#define SPI_REG_DATA *((volatile uint32_t *) 0x200000UL)
#define SPI_REG_STATUS *((volatile uint32_t *) 0x200004UL)
#define SPI_REG_SS *((volatile uint32_t *) 0x200008UL)

typedef enum SpiSlave SpiSlave;

enum SpiSlave {
	SPI_SLAVE_NONE,
	SPI_SLAVE_ROM,
	SPI_SLAVE_TGB,
	SPI_SLAVE_SD,
	SPI_SLAVE_EXT0,
	SPI_SLAVE_EXT1,
	SPI_SLAVE_EXT2,
	SPI_SLAVE_EXT3,
};

void spi_select_slave(int slave);
uint8_t spi_send_recv(uint8_t dat);
void spi_set_clockdiv(uint16_t clkdiv);

#endif
