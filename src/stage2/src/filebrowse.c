#include <stdint.h>
#include <stddef.h>
#include "filebrowse.h"
#include "fat.h"
#include "sd.h"
#include "printf.h"
#include "terminal.h"
#include "input.h"
#include "hexload.h"
#include "rom.h"
#include "mmu040.h"
#include "elf.h"
#include "util.h"
#include "main.h"

static const char *attribs = "RHSLDA";

static char pathbuf[256];
static char path[256] = "/";

static char dir[8][32];

static void clear_and_print_filename(void *arg);
static void execute_elf(void *arg);

void select_file(void *arg);
void select_file_action(void *arg);

void list_dir(void *arg);

int end;

typedef struct RomHeader RomHeader;
struct RomHeader {
	uint32_t magic;
	uint32_t entry;
	uint32_t size;
	uint32_t dest_addr;
	uint32_t flash_offset;
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

Menu menu_file= {
	clear_and_print_filename,
	"----------------------------------------\n",
	true,
	0,
	7,
	{
		{"Read as text", select_file_action, &menu_file.selected},
		{"Read as hex", select_file_action, &menu_file.selected},
		{"Execute HEX", select_file_action, &menu_file.selected},
		{"Execute ELF", execute_elf, &menu_file.selected},
		{"Display on screen", select_file_action, &menu_file.selected},
		{"Flash to boot ROM", select_file_action, &menu_file.selected},
		{"Play WAV file", select_file_action, &menu_file.selected},
	},
};


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

static void clear_and_print_filename(void *arg) {
	terminal_clear();
	printf("%s\n%s", path, arg);
}


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

typedef struct HexStat HexStat;
struct HexStat {
	uint32_t size;
	uint32_t offset;
};

static int _write_rom_byte(volatile uint8_t *addr, uint8_t data, void *arg) {
	HexStat *stat = arg;
	uint32_t rom_addr = ((uint32_t) addr) & (ROM_SIZE - 1);
	rom_write(rom_addr, &data, 1);
	stat->size++;
	if(rom_addr < stat->offset)
		stat->offset = rom_addr;
	return 0;
}

void load_hex_to_rom(const char *path) {
		void *entry;
		HexStat stat = {0, 0xFFFFFFFF};
		RomHeader header;
		int x, y;
		int fd;
		
		printf("Erasing ROM... ");
		
		rom_erase();
		while(rom_status() & 0x1);
		
		terminal_set_fg(TERMINAL_COLOR_LIGHT_GREEN);
		printf("done.\n");
		terminal_set_fg(TERMINAL_COLOR_LIGHT_GRAY);
		
		printf("Writing to ROM... ");
		terminal_get_pos(&x, &y);
		
		fd = fat_open(path, O_RDONLY);
		
		if(hexload(get_byte, fd, _write_rom_byte, &stat, &entry) < 0) {
			terminal_set_fg(TERMINAL_COLOR_RED);
			printf("Invalid hex file");
			terminal_set_fg(TERMINAL_COLOR_LIGHT_GRAY);
			
			fat_close(fd);
			return;
		}
		fat_close(fd);
		
		header.magic = 0xDEADBEEF;
		header.entry = (uint32_t) entry;
		header.size = stat.size;
		header.dest_addr = LLRAM_BASE | stat.offset;
		header.flash_offset = stat.offset;
		
		rom_write(ROM_SIZE - 512, (void *) &header, sizeof(header));
		
		terminal_set_pos(x, y);
		terminal_set_fg(TERMINAL_COLOR_LIGHT_GREEN);
		printf("done.\n");
		terminal_set_fg(TERMINAL_COLOR_LIGHT_GRAY);
		printf("Press any key...");
}

static void execute_elf(void *arg) {
	int fd, size, i, j;
	void *entry;
	//extern void *end;
	void *elf_begin = ((void *)(&end)) + 4*1024;
	uint8_t *tmp = elf_begin;
	
	printf("Load file to RAM\n");
	fd = fat_open(path, O_RDONLY);
	size = fat_fsize(fd);
	
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
	printf("File loaded\n");
	printf("Highest addr used is 0x%X\n", tmp);
	input_poll();
	
	mmu040_init();
	terminal_clear();
	printf("MMU Init\n");
	if(!(entry = elf_load(elf_begin))) {
		printf("Failed to load ELF\n");
		input_poll();
		return;
	}
	printf("ELF load successful, entry is 0x%X, press any key\n", entry);
	input_poll();
	//printf("Here we have 0x%X\n", *((uint32_t *) entry));
	//input_poll();
	mmu_disable();
	mmu_enable_and_jump(entry, 0, NULL);
}

void execute_elf_path(const char *_path) {
	strcpy(path, _path);
	execute_elf(NULL);
}

void select_file_action(void *arg) {
	int selected = *((int *) arg);
	int i, j, k, fd, size;
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
			{
				void *addr, *entry;
				fd = fat_open(path, O_RDONLY);
				if(hexload(get_byte, fd, hexload_write_byte, NULL, &entry) < 0) {
					printf("Invalid hex file");
					fat_close(fd);
					input_poll();
					break;
				}
				fat_close(fd);
				fd = fat_open(path, O_RDONLY);
				
				if(hexload(get_byte, fd, hexload_verify_byte, NULL, &addr) < 0) {
					printf("Hexload validate error at 0x%X\n", addr);
					fat_close(fd);
					input_poll();
					break;
				}
				
				__asm__ __volatile__ ("cpusha %dc\n");
				goto *addr;
				
				fat_close(fd);
				input_poll();
			}
			break;
		case 4:
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
		case 5:
			load_hex_to_rom(path);
			input_poll();
			
			break;
		
		case 6:
			terminal_clear();
			printf("Loading WAV to RAM\n");
			
			fd = fat_open(path, O_RDONLY);
			size = fat_fsize(fd);
			tmp = (volatile uint8_t *) SDRAM_BASE;
			
			printf("0/%u kB", size >> 10);
		
			for(j = 0; j < size; j += 512) {
				fat_read_sect(fd);
				for(i = 0; i < 512; i++) {
					*tmp++ = fat_buf[i];
				}
				printf("\r%u/%u kB", j >> 10, size >> 10);
			}
			fat_close(fd);
			printf("\n");
			wav_play((uint8_t *) SDRAM_BASE);
			printf("Press any key\n");
			input_poll();
			terminal_clear();
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
