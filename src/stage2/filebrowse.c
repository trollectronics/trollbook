#include "filebrowse.h"
#include "fat.h"
#include "sd.h"
#include "printf.h"
#include "terminal.h"
#include "input.h"
#include "hexload.h"
#include "rom.h"
#include "main.h"

static const char *attribs = "RHSLDA";

static char pathbuf[256];
static char path[256] = "/";

static char dir[8][32];

static void clear_and_print_filename(void *arg);

void select_file(void *arg);
void select_file_action(void *arg);

void list_dir(void *arg);

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
	5,
	{
		{"Read as text", select_file_action, &menu_file.selected},
		{"Read as hex", select_file_action, &menu_file.selected},
		{"Execute", select_file_action, &menu_file.selected},
		{"Display on screen", select_file_action, &menu_file.selected},
		{"Flash to boot ROM", select_file_action, &menu_file.selected},
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
			{
				void *addr;
				uint32_t ret;
				fd = fat_open(path, O_RDONLY);
				if((addr = hexload(get_byte, fd)) == (void *) 0xFFFFFFFF) {
					printf("Invalid hex file");
					fat_close(fd);
					input_poll();
					break;
				}
				fat_close(fd);
				fd = fat_open(path, O_RDONLY);
				switch(ret = hexload_validate(get_byte, fd)) {
					case -1:
						printf("Hexload general error\n");
						break;
					case 0:
						__asm__ __volatile__ ("cpusha %dc\n");
						goto *addr;
						break;
						
					default:
						printf("Hexload validate error at 0x%X\n", ret);
						break;
				};
				fat_close(fd);
				input_poll();
			}
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
