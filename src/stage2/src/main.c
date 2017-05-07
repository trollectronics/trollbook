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
#include "memtest.h"
#include "serial-transfer.h"
#include "main.h"

static void clear_and_print(void *arg);
static void test_spi_rom(void *arg);
static void test_sdram(void *arg);
static void test_lowres(void *arg);
static void autoboot(void *arg);
void color_demo(void *arg);
static void test_sound(void *arg);
void ui(void *arg);

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

Menu menu_main = {
	clear_and_print,
	"Trollectronics Trollbook BIOS\nMain menu\n----------------------------------------\n",
	false,
	0,
	11,
	{
		{"Boot kernel.elf", autoboot, NULL},
		{"Browse SD card filesystem", menu_execute, &menu_dir},
		{"Serial file transfer to SD", serial_transfer_recv, NULL},
		{"Test SPI ROM", test_spi_rom, NULL},
		{"Test Sound", test_sound, NULL},
		{"Test keyboard", input_test_keyboard, NULL},
		{"Test UI", ui, NULL},
		{"Test Low-res video mode", test_lowres, NULL},
		{"SDRAM Memtest", test_sdram, NULL},
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
	
	for(j = 0x5DD00; ; j += 256) {
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
	terminal_clear();
	
	printf("Performing memtest without the cache\n");
	memtest_run(false);
	printf("Performing memtest with the cache\n");
	memtest_run(true);
	
	input_poll();
}

void color_demo(void *arg) {
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

static void autoboot(void *arg) {
	execute_elf_path("/KERNEL.ELF");
}

static void test_sound(void *arg) {
	volatile uint16_t *buf= (volatile uint16_t *) LLRAM_BASE;
	volatile uint32_t *sound_hw = (volatile uint32_t *) PERIPHERAL_SOUND_BASE;
	int buffer;
	unsigned int i;
	
	for(i = 0; i < 1024; i++) {
		buf[i] = 0;
	}
	
	sound_hw[2] = 0x0;
	sound_hw[3] = 3;
	sound_hw[4] = 512;
	buffer = sound_hw[1] & 0x1;
	sound_hw[0] = 0x1;
	
	for(;;) {
		sound_wait();
		for(i = 0; i < 256; i++) {
			buf[i] = 0xFFFF;
		}
		sound_wait();
		for(i = 512; i < 768; i++) {
			buf[i] = 0xFFFF;
		}
		sound_wait();
		for(i = 0; i < 256; i++) {
			buf[i] = 0x0;
		}
		sound_wait();
		for(i = 512; i < 768; i++) {
			buf[i] = 0x0;
		}
	}
}

static void test_lowres(void *arg) {
	volatile uint8_t *buf= (volatile uint8_t *) LLRAM_BASE;
	volatile uint32_t *vga_hw = (volatile uint32_t *) PERIPHERAL_VGA_BASE;
	int i;
	
	//~ for(i = 0; i < 400*240/2; i++) {
		//~ buf[i] = 0x1;
		//~ buf[i + 400] = 0x2;
	//~ }
	
	//~ for(i = 0; i < 400*240/2; i++) {
		//~ buf[i + 128*1024] = 0x4;
		//~ buf[i + 400 + 128*1024] = 0x5;
	//~ }
	
	input_poll();
	
	vga_hw[0] = 0x3;
	input_poll();
	vga_hw[0] = 0x7;
	input_poll();
	
	vga_hw[0] = 0x1;
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
	
	//ui();
	menu_execute(&menu_main);
	
	for(;;);
	
	fail:
	terminal_set_fg(TERMINAL_COLOR_LIGHT_RED);
	printf("failed");
	terminal_set_fg(TERMINAL_COLOR_WHITE);
	
	for(;;);
		
	return 0;
}


