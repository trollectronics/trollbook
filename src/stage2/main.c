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

void reboot(void *);
void select_file(void *arg);
void select_file_action(void *arg);

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
	
	if(!(s1[0] == '/' && s1[1] == 0))
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

static char dir[8][32];

void list_dir(void *arg) {
	terminal_clear();
	printf("%s\n", path);
	printf("%s", arg);
	
	int files, fd, i, j, k;
	
	for(i = 0; i < 8; i++) {
		dir[i][0] = 0;
	}
	
	uint8_t stat;
	struct FATDirList list[8];
	char *buf, *tmp;
	for(files = 0; (i = fat_dirlist(path, list, 8, files)); files += i) {
		for (j = i - 1; j >= 0; j--) {
			if(list[j].filename[0]) {
				stat = list[j].attrib;
				
				//skip volume labels
				if(stat & 0x8)
					continue;
				
				if(list[j].filename[0] == '.')
					continue;
				
				buf = dir[j];
				
				for(k = 5; k != ~0; k--) {
					if(stat & (0x1 << k))
						*buf++ = attribs[k];
					else
						*buf++ = '-';
				}
				if(stat & 0x10) {
					*buf++ = '\t';
					*buf++ = '\t';
				} else {
					pathcat((char *) pathbuf, path, list[j].filename);
					fd = fat_open(pathbuf, O_RDONLY);
					*buf++ = '\t';
					//print_filesize(fat_fsize(fd));
					*buf++ = '0';
					*buf++ = '\t';
					fat_close(fd);
				}
				tmp = list[j].filename;
				
				while(*tmp) {
					*buf++ = *tmp++;
				}
				*buf = 0;
			}
		}
	}
}

void clear_and_print_filename(void *arg) {
	terminal_clear();
	printf("%s\n%s", path, arg);
}

void test_spi_rom(void *arg) {
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
	printf("%s", arg);
}


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

Menu menu_file= {
	clear_and_print_filename,
	"----------------------------------------\n",
	true,
	0,
	5,
	{
		{"Read as text", select_file_action, &menu_file.selected},
		{"Read as hex", select_file_action, &menu_file.selected},
		{"Execute", select_file_action, &menu_file.selected},
		{"Display on screen", select_file_action, &menu_file.selected},
		{"Flash to boot ROM", select_file_action, &menu_file.selected},
	},
};

Menu menu_dir = {
	list_dir,
	"Attrib | Size  | Name\n-------+-------+----------------\n",
	true,
	0,
	8,
	{
		{dir[0], select_file, &menu_dir.selected},
		{dir[1], select_file, &menu_dir.selected},
		{dir[2], select_file, &menu_dir.selected},
		{dir[3], select_file, &menu_dir.selected},
		{dir[4], select_file, &menu_dir.selected},
		{dir[5], select_file, &menu_dir.selected},
		{dir[6], select_file, &menu_dir.selected},
		{dir[7], select_file, &menu_dir.selected},
	},
};

Menu menu_main = {
	clear_and_print,
	"Trollectronics Trollbook BIOS\nMain menu\n----------------------------------------\n",
	false,
	0,
	5,
	{
		{"Sub menu test", menu_execute, &menu_sub},
		{"Browse SD card filesystem", menu_execute, &menu_dir},
		{"Test SPI ROM", test_spi_rom, NULL},
		{"Color demo", color_demo, NULL},
		{"Reboot", reboot, NULL},
	},
};

#define MAX(a, b) ((a) > (b) ? (a) : (b))

static uint8_t get_byte(int fd) {
	static unsigned int count = 0;
	uint8_t c;
	
	if(fd < 0) {
		count = 0;
		return 0;
	}
	
	if(!count)
		fat_read_sect(fd);
	
	c = fat_buf[count];
	
	if(++count == 512)
		count = 0;
	
	return c;
}

void select_file_action(void *arg) {
	int selected = *((int *) arg);
	int i, j, k, fd, size, x, y;
	volatile uint8_t *tmp;
	
	switch(selected) {
		case 0:
			fd = fat_open(path, O_RDONLY);
			size = fat_fsize(fd);
	
			for(j = 0; j < size; j += 512) {
				terminal_clear();
				fat_read_sect(fd);
				for(i = 0; i < 512; i++) {
					if(fat_buf[i] < 32 || fat_buf[i] > 126)
						printf("?");
					else
						printf("%c", fat_buf[i]);
					
				}
				input_poll();
			}
			fat_close(fd);
			break;
		case 1:
			fd = fat_open(path, O_RDONLY);
			size = fat_fsize(fd);
	
			for(j = 0; j < size; j += 512) {
				terminal_clear();
				printf("\nSector %u\n", j >> 9);
				fat_read_sect(fd);
				for(i = 0; i < 256; i+=16) {
					tmp =((uint8_t *) fat_buf) + i;
					printf("%04x\t%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\t\t", 
						j + i,
						tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6], tmp[7], 
						tmp[8], tmp[9], tmp[10], tmp[11], tmp[12], tmp[13], tmp[14], tmp[15]
					);
					for(k = 0; k < 16; k++) {
						if(tmp[k] < 32 || tmp[k] > 126)
							printf(".");
						else
							printf("%c", tmp[k]);
					}
					printf("\n");
					
				}
				input_poll();
				terminal_clear();
				printf("\nSector %u\n", j >> 9);
				for(i = 256; i < 512; i+=16) {
					tmp =((uint8_t *) fat_buf) + i;
					printf("%04x\t%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\t\t", 
						j + i,
						tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6], tmp[7], 
						tmp[8], tmp[9], tmp[10], tmp[11], tmp[12], tmp[13], tmp[14], tmp[15]
					);
					for(k = 0; k < 16; k++) {
						if(tmp[k] < 32 || tmp[k] > 126)
							printf(".");
						else
							printf("%c", tmp[k]);
					}
					printf("\n");
					
				}
				input_poll();
			}
			fat_close(fd);

			break;
		case 2:
			fd = fat_open(path, O_RDONLY);
			hexload(get_byte, fd);
			printf("Error parsing HEX file\n");
			fat_close(fd);
			input_poll();
			break;
		case 3:
			fd = fat_open(path, O_RDONLY);
			size = fat_fsize(fd);
			tmp = MEM_VGA_RAM;
			
			for(j = 512; j < size; j += 512) {
				fat_read_sect(fd);
				for(i = 0; i < 512; i++) {
					*tmp++ = fat_buf[i];
				}
			}
			fat_read_sect(fd);
			for(i = 0; i < j - size; i++) {
				*tmp++ = fat_buf[i];
			}
			
			fat_close(fd);
			input_poll();
			terminal_clear();
			break;
		case 4:
			printf("Erasing ROM... ");
			
			rom_erase();
			while(rom_status() & 0x1);
			
			terminal_set_fg(TERMINAL_COLOR_LIGHT_GREEN);
			printf("done.\n");
			terminal_set_fg(TERMINAL_COLOR_LIGHT_GRAY);
			
			printf("Writing to ROM... ");
			terminal_get_pos(&x, &y);
			
			fd = fat_open(path, O_RDONLY);
			size = fat_fsize(fd);
	
			for(j = 0; j < size; j += 512) {
				terminal_set_pos(x, y);
				printf("sector %u                 ", j >> 9);
				fat_read_sect(fd);
				rom_write(j, (void *) fat_buf, 512);
			}
			if(size & 0x1FF) {
				terminal_set_pos(x, y);
				printf("sector %u                 ", j >> 9);
				fat_read_sect(fd);
				rom_write(j, (void *) fat_buf, size & 0x1FF);
			}
			
			fat_close(fd);
			
			terminal_set_pos(x, y);
			terminal_set_fg(TERMINAL_COLOR_LIGHT_GREEN);
			printf("done.                       \n");
			terminal_set_fg(TERMINAL_COLOR_LIGHT_GRAY);
			input_poll();
			
			break;
	}
}

void select_file(void *arg) {
	int selected = *((int *) arg);
	char *tmp, *tmp2;
	
	tmp = dir[selected];
	while(*tmp++ != '\t');
	while(*tmp++ != '\t');
	
	pathcat(pathbuf, path, tmp);
	tmp = pathbuf;
	tmp2 = path;
	while((*tmp2++ = *tmp++));
	
	if(dir[selected][1] == 'D') {
		menu_dir.selected = 8;
		menu_execute(&menu_dir);
		tmp = path;
		while(*tmp++);
		while(tmp != path) {
			*tmp = 0;
			if(*tmp == '/') {
				break;
			}
			tmp--;
		}
	} else {
		menu_dir.selected = 0;
		menu_execute(&menu_file);
		
		tmp = path;
		while(*tmp++);
		while(tmp != path) {
			*tmp = 0;
			if(*tmp == '/') {
				break;
			}
			tmp--;
		}
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


