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

static unsigned char *ascii_filter(unsigned char *s) {
	static unsigned char ret[17];
	int i = 0;
	while(i < 16) {
		ret[i] = (*s < 32 || *s > 126)? '.' : (*s);
		i++;
		s++;
	}
	return ret;
}

static void print_sector(uint32_t sector) {
	SDStreamStatus status;
	int i;
	unsigned int count;
	uint8_t data[17];
	
	data[16] = 0;
	status = SD_STREAM_STATUS_BEGIN;
	i=0;
	printf("\nSector %u\n", sector);
	sd_stream_read_block(&status, sector);
	if(status == SD_STREAM_STATUS_FAILED) {
		printf("Failed\n");
		return;
	}
	count = 0;
	while(status >= 1) {
		data[i++] = sd_stream_read_block(&status);
		if(i == 16) {
			printf("|%04x|%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x|%s\n", 
				count, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], 
				data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15],
				ascii_filter(data)
			);
			i = 0;
			count += 16;
		}
	}
	if(status == SD_STREAM_STATUS_FAILED) {
		printf("Failed\n");
		return;
	}
}

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

int main() {
	uint8_t b[2];
	int type;
	char label[12];
	
	term_init();
	
	printf("Detecting SD card: ");
	if((type = sd_init()) == SD_CARD_TYPE_INVALID) {
		term_puts("failed", 4 + 8);
		goto fail;
	}
	
	term_puts(sd_card_type_name[type], 2 + 8);
	term_putc_term('\n', 15);
	printf(" - Card size: %u kB\n", sd_get_card_size()/2);
	
	printf("Detecting file system: ");
	if(fat_init(fat_read_sd, fat_write_sd, fat_buf) < 0) {
		term_puts("failed", 4 + 8);
		goto fail;
	}
	
	type = fat_type();
	term_puts(fat_type_name[type], 2 + 8);
	term_putc_term('\n', 15);
	fat_get_label(label);
	printf(" - Volume label: %s\n\n", label);
	
	
	printf("Attrib | Size  | Name\n");
	printf("-------+-------+----------------\n");
	
	int files, fd, i, j, k;
	uint8_t stat;
	struct FATDirList list[8];
	for(files = 0; (i = fat_dirlist(path, list, 8, files)); files += i) {
		for (j = i - 1; j >= 0; j--) {
			if(list[j].filename[0]) {
				stat = list[j].attrib;
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
	
	for(;;);
	
	unsigned int lol, col = 0;
	for(;;) {
		for(lol = 0; lol < 203; lol++)
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


