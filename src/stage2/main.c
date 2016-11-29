#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "uart.h"
#include "spi.h"
#include "printf.h"
#include "terminal.h"
#include "menu.h"
#include "sd.h"
#include "input.h"
#include "hexload.h"
#include "rom.h"
#include "fat.h"
#include "cache.h"
#include "filebrowse.h"
#include "main.h"

static void clear_and_print(void *arg);
static void test_spi_rom(void *arg);
static void test_sdram(void *arg);
static void color_demo(void *arg);

uint8_t fat_buf[512];

static const char *sd_card_type_name[] = {
	[SD_CARD_TYPE_MMC] = "MMC",
	[SD_CARD_TYPE_SD] = "SD",
	[SD_CARD_TYPE_SDHC] = "SDHC",
};

static const char *fat_type_name[] = {
	[FAT_TYPE_FAT16] = "FAT16",
	[FAT_TYPE_FAT32] = "FAT32",
};

Menu menu_sub= {
	clear_and_print,
	"Sub menu test\n----------------------------------------\n",
	true,
	0,
	2,
	{
		{"HEHEUHEHAHEAHUEH", NULL, NULL},
		{"HEYEAYEAYEA!", NULL, NULL},
	},
};

Menu menu_main = {
	clear_and_print,
	"Trollectronics Trollbook BIOS\nMain menu\n----------------------------------------\n",
	false,
	0,
	6,
	{
		{"Sub menu test", menu_execute, &menu_sub},
		{"Browse SD card filesystem", menu_execute, &menu_dir},
		{"Test SPI ROM", test_spi_rom, NULL},
		{"Test SDRAM", test_sdram, NULL},
		{"Color demo", color_demo, NULL},
		{"Reboot", reboot, NULL},
	},
};


static int fat_read_sd(uint32_t sector, uint8_t *data) {
	SDStreamStatus status;
	
	status = SD_STREAM_STATUS_BEGIN;
	
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
	
	status = SD_STREAM_STATUS_BEGIN;
	
	sd_stream_read_block(&status, sector);
	if(status == SD_STREAM_STATUS_FAILED)
		return -1;
	
	while(status >= 1)
		sd_stream_read_block(&status, *data++);
	
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

static void clear_and_print(void *arg) {
	static bool clear = false;
	
	if(clear)
		terminal_clear();
	
	clear = true;
	printf("%s", arg);
}

static void test_spi_rom(void *arg) {
	uint8_t buf[256];
	uint8_t *tmp;
	int i, j, k;
	
	for(j = 0; ; j += 256) {
		terminal_clear();
		rom_read(j, buf, 256);
		for(i = 0; i < 256; i+=16) {
			tmp =((uint8_t *) buf) + i;
			printf("%04x\t%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\t\t", 
				j + i,
				tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6], tmp[7], 
				tmp[8], tmp[9], tmp[10], tmp[11], tmp[12], tmp[13], tmp[14], tmp[15]
			);
			for(k = 0; k < 16; k++) {
				if(tmp[k] < 32 || tmp[k] > 126)
					printf("%c", '.');
				else
					printf("%c", tmp[k]);
			}
			printf("\n");
		}
		
		input_poll();
	}
}

static void test_sdram(void *arg) {
	volatile uint8_t *test = (void *) 0x40000000UL;
	volatile uint8_t *loltest = (void *) 0x40000000UL;
	volatile uint8_t *tmp;
	int k;
	//extern uint32_t *hejtest;
	//uint32_t *test = hejtest;
	int i;
	
	for(i = 0; i < 256; i++) {
		*test++ = i;
	}
	
	//__asm__ __volatile__ ("cpusha %dc\n");
	
	terminal_clear();
	for(i = 0; i < 256; i+=16) {
		tmp =((uint8_t *) loltest) + i;
		printf("%04x\t%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\t\t", 
			i,
			tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6], tmp[7], 
			tmp[8], tmp[9], tmp[10], tmp[11], tmp[12], tmp[13], tmp[14], tmp[15]
		);
		for(k = 0; k < 16; k++) {
			if(tmp[k] < 32 || tmp[k] > 126)
				printf("%c", '.');
			else
				printf("%c", tmp[k]);
		}
		printf("\n");
	}

	input_poll();
	
	input_poll();
}

static void color_demo(void *arg) {
	unsigned int i, col = 0;
	for(;;) {
		for(i = 0; i < 230; i++) {
			terminal_set_fg(col++);
			terminal_puts("Hello, world ");
		}
		terminal_set_fg(col++);
		terminal_puts("Hello, wor");
		terminal_set_pos(0, 0);
	}
}

int main() {
	int type;
	char label[12];
	
	terminal_init();
	
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
	
	menu_execute(&menu_main);
	
	for(;;);
	
	fail:
	terminal_set_fg(TERMINAL_COLOR_LIGHT_RED);
	printf("failed");
	terminal_set_fg(TERMINAL_COLOR_WHITE);
	
	for(;;);
		
	return 0;
}


