#ifndef SPI_H_
#define SPI_H_

#include "stdint.h"

void spi_init();
void spi_deinit();
void spi_send(uint8_t data);
uint8_t spi_recv();

#endif
