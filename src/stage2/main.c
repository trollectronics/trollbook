#include <stdint.h>
#include <stddef.h>
#include "uart.h"
#include "spi.h"
#include "printf.h"
#include "boot_term.h"
#include "sd.h"
#include "fat.h"

static uint8_t fat_buf[512];

static int fat_read_sd(uint32_t sector, uint8_t *data) {
	SDStreamStatus status;
	
	sd_stream_read_block(&status, sector);
	if(status == SD_STREAM_STATUS_FAILED)
		return -1;
	
	while(status >= 1)
		*data++ = sd_stream_read_block(&status);
	
	if(status == SD_STREAM_STATUS_FAILED)
		return -1;
	
	return 0;
}

static int fat_write_sd(uint32_t sector, uint8_t *data) {
	SDStreamStatus status;
	
	sd_stream_read_block(&status, sector);
	if(status == SD_STREAM_STATUS_FAILED)
		return -1;
	
	while(status >= 1)
		sd_stream_read_block(&status, *data++);
	
	if(status == SD_STREAM_STATUS_FAILED)
		return -1;
	
	return 0;
}

static const char *sd_card_type_name[] = {
	[SD_CARD_TYPE_MMC] = "MMC",
	[SD_CARD_TYPE_SD] = "SD",
	[SD_CARD_TYPE_SDHC] = "SDHC",
};

static const char *fat_type_name[] = {
	[FAT_TYPE_FAT16] = "FAT16",
	[FAT_TYPE_FAT32] = "FAT32",
};

int main() {
	uint8_t b[2];
	int type;
	char label[12];
	
	term_init();
	
	printf("Detecting SD card: ");
	if((type = sd_init()) == SD_CARD_TYPE_INVALID) {
		term_puts("failed", 4);
		goto fail;
	}
	
	term_puts(sd_card_type_name[type], 2);
	term_putc_term('\n', 15);
	printf(" - Card size: %u\n", sd_get_card_size());
	
	printf("Detecting file system: ");
	if(fat_init(fat_read_sd, fat_write_sd, fat_buf) < 0) {
		term_puts("failed", 4);
		goto fail;
	}
	
	type = fat_type();
	term_puts(fat_type_name[type], 2);
	term_putc_term('\n', 15);
	fat_get_label(label);
	printf(" - Volume label: %s\n", label);
	
	for(;;);
	
	unsigned int i, col = 0;
	for(;;) {
		for(i = 0; i < 203; i++)
			term_puts("Hello, world ", (col++) % 256);
		
		term_set_pos(0, 0);
	}
	
	
	
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
	
	fail:
	for(;;);
		
	return 0;
}


