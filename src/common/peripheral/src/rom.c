#include <stdint.h>
#include "spi.h"
#include "rom.h"

uint8_t rom_status() {
	uint8_t status;
	spi_select_slave(SPI_SLAVE_ROM);
	
	spi_send_recv(ROM_COMMAND_READ_STATUS);
	status = spi_send_recv(0xFF);
	
	spi_select_slave(SPI_SLAVE_NONE);
	
	return status;
}

void rom_write_enable() {
	while(rom_status() & 0x1);
	spi_select_slave(SPI_SLAVE_ROM);
	spi_send_recv(ROM_COMMAND_WRITE_ENABLE);
	spi_select_slave(SPI_SLAVE_NONE);
}

void rom_erase() {
	rom_write_enable();
	
	while(rom_status() & 0x1);
	spi_select_slave(SPI_SLAVE_ROM);
	spi_send_recv(ROM_COMMAND_ERASE_CHIP);
	spi_select_slave(SPI_SLAVE_NONE);
}

void rom_write(uint32_t address, const uint8_t *buffer, uint32_t size) {
	int i;
	
	while(size >= 256) {
		rom_write_enable();
		
		while(rom_status() & 0x1);
		spi_select_slave(SPI_SLAVE_ROM);
		spi_send_recv(ROM_COMMAND_PAGE_PROGRAM);
		spi_send_recv((address >> 16) & 0xFF);
		spi_send_recv((address >> 8) & 0xFF);
		spi_send_recv(address & 0xFF);
		
		for(i = 0; i < 256; i++)
			spi_send_recv(*buffer++);
		
		spi_select_slave(SPI_SLAVE_NONE);
		
		size -= 256;
		address += 256;
	}
	if(size) {
		rom_write_enable();
		
		while(rom_status() & 0x1);
		spi_select_slave(SPI_SLAVE_ROM);
		spi_send_recv(ROM_COMMAND_PAGE_PROGRAM);
		spi_send_recv((address >> 16) & 0xFF);
		spi_send_recv((address >> 8) & 0xFF);
		spi_send_recv(address & 0xFF);
		
		for(i = 0; i < size; i++)
			spi_send_recv(*buffer++);
		
		spi_select_slave(SPI_SLAVE_NONE);
	}
}

void rom_read(uint32_t address, uint8_t *buffer, uint32_t size) {
	while(rom_status() & 0x1);
	
	spi_select_slave(SPI_SLAVE_ROM);
	spi_send_recv(ROM_COMMAND_READ_DATA);
	spi_send_recv((address >> 16) & 0xFF);
	spi_send_recv((address >> 8) & 0xFF);
	spi_send_recv(address & 0xFF);
	
	while(size--)
		*buffer++ = spi_send_recv(0xFF);
	
	spi_select_slave(SPI_SLAVE_NONE);	
}
