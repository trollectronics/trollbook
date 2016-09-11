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
#include "fat.h"

void reboot(void *);

static uint8_t fat_buf[512];

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

static const char *sd_card_type_name[] = {
	[SD_CARD_TYPE_MMC] = "MMC",
	[SD_CARD_TYPE_SD] = "SD",
	[SD_CARD_TYPE_SDHC] = "SDHC",
};

static const char *fat_type_name[] = {
	[FAT_TYPE_FAT16] = "FAT16",
	[FAT_TYPE_FAT32] = "FAT32",
};

const char *attribs = "RHSLDA";

void pathcat(char *buf, const char *s1, const char *s2) {
	char c;
	while((c = *s1++))
		*buf++ = c;
	*buf++ = '/';
	while((c = *s2++))
		*buf++ = c;
	*buf++ = 0;
}


static void print_filesize(uint32_t filesize) {
	if(filesize < 1024)
		printf("%u", filesize);
	else if(filesize < 1024*1024)
		printf("%uk", filesize/1024U);
	else
		printf("%uM", filesize/(1024U*1024U));
}

static char pathbuf[256];
static char path[256] = "/";

void list_dir(void *arg) {
	terminal_clear();
	printf("Attrib | Size  | Name\n");
	printf("-------+-------+----------------\n");
	
	int files, fd, i, j, k;
	uint8_t stat;
	struct FATDirList list[8];
	for(files = 0; (i = fat_dirlist(path, list, 8, files)); files += i) {
		for (j = i - 1; j >= 0; j--) {
			if(list[j].filename[0]) {
				stat = list[j].attrib;
				
				//skip volume labels
				if(stat & 0x8)
					continue;
				
				for(k = 5; k != ~0; k--) {
					if(stat & (0x1 << k))
						printf("%c", attribs[k]);
					else
						printf("-");
				}
				if(stat & 0x10) {
					printf("\t\t");
				} else {
					pathcat((char *) pathbuf, path, list[j].filename);
					fd = fat_open(pathbuf, O_RDONLY);
					printf("\t");
					print_filesize(fat_fsize(fd));
					printf("\t");
					fat_close(fd);
				}
				printf("%s\n", list[j].filename);
			}
		}
	}
	
	input_poll();
}

void test_spi_rom(void *arg) {
	uint8_t b[2];
	terminal_clear();
	spi_select_slave(1);
	spi_send_recv(0x90);
	spi_send_recv(0x0);
	spi_send_recv(0x0);
	spi_send_recv(0x0);
	b[0] = spi_send_recv(0xFF);
	b[1] = spi_send_recv(0xFF);
	
	if(b[0] == 0x1 && b[1] == 0x12) {
		terminal_set_fg(TERMINAL_COLOR_LIGHT_GREEN);
		printf("OK\n");
	} else {
		terminal_set_fg(TERMINAL_COLOR_LIGHT_RED);
		printf("Fail\n");
	}
	
	terminal_set_fg(TERMINAL_COLOR_LIGHT_GRAY);
	printf("Reply from SPI ROM: 0x%X 0x%X\n", b[0], b[1]);
	
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

static void clear_and_print(void *arg) {
	static bool clear = false;
	
	if(clear)
		terminal_clear();
	
	clear = true;
	printf(arg);
}


Menu menu_sub= {
	clear_and_print,
	"Sub menu test\n",
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
	"Trollectronics Trollbook BIOS\nMain menu\n",
	false,
	0,
	5,
	{
		{"Sub menu test", menu_execute, &menu_sub},
		{"List SD card filesystem", list_dir, NULL},
		{"Test SPI ROM", test_spi_rom, NULL},
		{"Color demo", color_demo, NULL},
		{"Reboot", reboot, NULL},
	},
};

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


