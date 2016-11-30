#ifndef _ROM_H_
#define _ROM_H_

#include <stdint.h>

#define ROM_SIZE 0x80000

typedef enum RomCommand RomCommand;
enum RomCommand {
	ROM_COMMAND_WRITE_ENABLE = 0x6,
	ROM_COMMAND_WRITE_DISABLE = 0x4,
	ROM_COMMAND_READ_STATUS = 0x5,
	ROM_COMMAND_WRITE_STATUS = 0x1,
	ROM_COMMAND_READ_DATA = 0x3,
	ROM_COMMAND_READ_DATA_FAST = 0xB,
	ROM_COMMAND_READ_DATA_FAST_DUAL = 0x3B,
	ROM_COMMAND_PAGE_PROGRAM = 0x2,
	ROM_COMMAND_ERASE_BLOCK = 0xD8,
	ROM_COMMAND_ERASE_SECTOR = 0x20,
	ROM_COMMAND_ERASE_CHIP = 0xC7,
	ROM_COMMAND_POWER_DOWN = 0xB9,
	ROM_COMMAND_DEVICE_ID = 0xAB,
	ROM_COMMAND_MANUFACTURER = 0x90,
	ROM_COMMAND_JEDEC_ID = 0x9F,
};

uint8_t rom_status();
void rom_write_enable();
void rom_erase();
void rom_write(uint32_t address, const uint8_t *buffer, uint32_t size);
void rom_read(uint32_t address, uint8_t *buffer, uint32_t size);

#endif
