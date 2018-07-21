#include <stdint.h>
#include <font.h>
#include <printf.h>
#include <terminal.h>
#include <interrupt.h>
#include <sd.h>
#include <fat.h>
#include <delay.h>
#include "gfx.h"
#include "main.h"

uint8_t fat_buf[512];

struct {
	uint8_t tj0[64000];
	uint8_t h3y[64000];
} data;

static const char *sd_card_type_name[] = {
	[SD_CARD_TYPE_MMC] = "MMC",
	[SD_CARD_TYPE_SD] = "SD",
	[SD_CARD_TYPE_SDHC] = "SDHC",
};

static const char *fat_type_name[] = {
	[FAT_TYPE_FAT16] = "FAT16",
	[FAT_TYPE_FAT32] = "FAT32",
};

static int fat_read_sd(uint32_t sector, uint8_t *data) {
	SDStreamStatus status;
	
	status = SD_STREAM_STATUS_BEGIN;
	
	sd_stream_read_block(&status, sector);
	if(status == SD_STREAM_STATUS_FAILED) {
		printf("sd_stream_read_block failed at SD_STREAM_STATUS_BEGIN\n");
		return -1;
	}
	
	while(status >= 1)
		*data++ = sd_stream_read_block(&status);
	
	if(status == SD_STREAM_STATUS_FAILED) {
		printf("sd_stream_read_block failed\n");
		return -1;
	}
	
	return 0;
}

static int fat_write_sd(uint32_t sector, uint8_t *data) {
	SDStreamStatus status;
	
	status = SD_STREAM_STATUS_BEGIN;
	
	sd_stream_write_block(&status, sector);
	if(status == SD_STREAM_STATUS_FAILED)
		return -1;
	
	while(status >= 1)
		sd_stream_write_block(&status, *data++);
	
	if(status == SD_STREAM_STATUS_FAILED)
		return -1;
	
	return 0;
}

static void print_filesize(uint32_t filesize) {
	if(filesize < 1024)
		printf("%u", filesize);
	else if(filesize < 1024*1024)
		printf("%uk", filesize/1024U);
	else
		printf("%uM", filesize/(1024U*1024U));
}

int rand() {
	//chosen by fair dice roll
	//guaranteed to be random
	return 4;
}

void load_file(const char *filename, void *buf) {
	uint32_t *src, *dst;
	unsigned i, j;
	unsigned size;
	int fd;
	
	printf("open file %s\n", filename);
	fd = fat_open(filename, O_RDONLY);
	if(fd < 0) {
		printf("failed to open file\n");
		return;
	}
	
	size = fat_fsize(fd);
	
	dst = buf;
	for(j = 0; j < size; j += 512) {
		fat_read_sect(fd);
		src = (uint32_t *) fat_buf;
		for(i = 0; i < 512/4; i++) {
			*dst++ = *src++;
		}
		printf("\r%u/%u kB", j >> 10, size >> 10);
	}
	fat_close(fd);
	printf("\n");
}

void load_files() {
	load_file("/DATA/TJ0.DAT", data.tj0);
	load_file("/DATA/H3Y.DAT", data.h3y);
}

int main(int argc, char **argv) {
	int type;
	char label[12];
	
	interrupt_init();
	
	terminal_init();
	terminal_clear();
	
	printf("Detecting SD card: ");
	if((type = sd_init()) == SD_CARD_TYPE_INVALID) {
		goto fail;
	}
	
	terminal_set_fg(TERMINAL_COLOR_LIGHT_GREEN);
	printf("%s\n", sd_card_type_name[type]);
	terminal_set_fg(TERMINAL_COLOR_WHITE);
	printf(" - Card size: ");
	print_filesize(sd_get_card_size()/2*1024);
	printf("B\n");
	
	printf("Detecting file system: ");
	if(fat_init(fat_read_sd, fat_write_sd, fat_buf) < 0) {
		goto fail;
	}
	
	type = fat_type();
	terminal_set_fg(TERMINAL_COLOR_LIGHT_GREEN);
	printf("%s\n", fat_type_name[type]);
	terminal_set_fg(TERMINAL_COLOR_WHITE);
	fat_get_label(label);
	printf(" - Volume label: %s\n\n", label);
	
	
	//load_files();
	delay_timer_set_prescale(30000);
	//gfx_set_lowres();
	for(;;) {
		//gfx_blit_fast(data.tj0, 320, 200, 40, 20);
		printf("tj0\n");
		delay_timer(3, 1000);
		//gfx_blit_fast(data.h3y, 320, 200, 40, 20);
		printf("h3y\n");
		delay_timer(3, 1000);
	}
	
	for(;;);
	
	fail:
	terminal_set_fg(TERMINAL_COLOR_LIGHT_RED);
	printf("failed");
	terminal_set_fg(TERMINAL_COLOR_WHITE);
	for(;;);
	
	return 0;
}

