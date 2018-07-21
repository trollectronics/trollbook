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

DemoData data;

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
	
	printf("loading file %s\n", filename);
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
		//printf("\r%u/%u kB", j >> 10, size >> 10);
	}
	fat_close(fd);
	//printf("\n");
}

void load_files() {
	int fd;
	uint32_t size;
	
	fd = fat_open("/DATA/DANS.MOD", O_RDONLY);
	size = fat_fsize(fd);
	data.modfile = malloc(size);
	data.modsize = size;
	fat_close(fd);
	
	load_file("/DATA/DANS.MOD", data.modfile);
	
	
	load_file("/DATA/A1.DAT", data.a1);
	load_file("/DATA/A2.DAT", data.a2);
	load_file("/DATA/AM.DAT", data.am);
	load_file("/DATA/AND.DAT", data.and);
	load_file("/DATA/B1.DAT", data.b1);
	load_file("/DATA/B2.DAT", data.b2);
	load_file("/DATA/BEJ.DAT", data.bej);
	load_file("/DATA/C1.DAT", data.c1);
	load_file("/DATA/C2.DAT", data.c2);
	load_file("/DATA/C3.DAT", data.c3);
	load_file("/DATA/C4.DAT", data.c4);
	load_file("/DATA/C5.DAT", data.c5);
	load_file("/DATA/FIND.DAT", data.find);
	load_file("/DATA/H3Y.DAT", data.h3y);
	load_file("/DATA/HIM.DAT", data.him);
	load_file("/DATA/JADER.DAT", data.jader);
	load_file("/DATA/KNOW.DAT", data.know);
	load_file("/DATA/KRAFT.DAT", data.kraft);
	load_file("/DATA/ME.DAT", data.me);
	load_file("/DATA/MIST.DAT", data.mist);
	load_file("/DATA/MLARGE.DAT", data.mlarge);
	load_file("/DATA/MSMALL.DAT", data.msmall);
	load_file("/DATA/TERROR.DAT", data.terror);
	load_file("/DATA/TJ0.DAT", data.tj0);
	load_file("/DATA/TSKO.DAT", data.tsko);
	load_file("/DATA/WANT.DAT", data.want);
	load_file("/DATA/YOU.DAT", data.you);
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
	
	delay_timer_set_prescale(30000);
	
	terminal_clear();
	printf("Introducing Trollbook\n");
	delay_timer(3, 500);
	printf("Completely custom designed and built computer\n");
	delay_timer(3, 500);
	printf(" - Motorola 68040 @ 33 MHz\n");
	delay_timer(3, 500);
	printf(" - 64 MB RAM\n");
	delay_timer(3, 500);
	printf(" - 800x480 LCD display\n");
	delay_timer(3, 500);
	printf(" - Stereo 8 bit PCM sound\n");
	delay_timer(3, 500);
	printf(" - Palmtop form factor, built in keyboard and mouse\n");
	delay_timer(3, 500);
	printf(" - Completely custom graphics and sound controller in FPGA\n");
	delay_timer(3, 500);
	printf(" - Custom designed PCBs\n");
	delay_timer(3, 500);
	printf(" - All FPGA, BIOS and OS code from scratch\n");
	delay_timer(3, 2000);
	printf("\n");
	printf("First demo running on the device\n");
	delay_timer(3, 500);
	printf("We ported an old...\n");
	delay_timer(3, 500);
	printf("CLASSIC...\n");
	delay_timer(3, 500);
	printf("\n\n\n");
	
	printf("h4xxel & slaeshjag of //achtung fulkod\n");
	printf("\n");
	delay_timer(3, 5000);
	
	terminal_clear();
	load_files();
	terminal_clear();
	gfx_set_lowres();
	
	demo();
	
	for(;;);
	
	fail:
	terminal_set_fg(TERMINAL_COLOR_LIGHT_RED);
	printf("failed");
	terminal_set_fg(TERMINAL_COLOR_WHITE);
	for(;;);
	
	return 0;
}

