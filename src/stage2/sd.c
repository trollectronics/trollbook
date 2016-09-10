#include <stdint.h>
#include <stdarg.h>
#include "sd.h"
#include "spi.h"
#include "delay.h"

static SDCardType type = SD_CARD_TYPE_INVALID;

SDCardType sd_init(void) {
	unsigned int i, j;
	
	spi_set_clockdiv(165);
	
	spi_select_slave(SPI_SLAVE_NONE);
	for(i = 0; i < 12; i++)
		spi_send_recv(0xFF);
	spi_select_slave(SPI_SLAVE_SD);
	
	/*Finish block operation in case of reset*/
	for(i = 0; i < 520; i++)
		spi_send_recv(0xFF);
	
	j = 0;
	do {
		if(j++ > 200)
			return (type = SD_CARD_TYPE_INVALID);
		spi_send_recv(0xFF);
		spi_send_recv(0xFF);
		spi_send_recv(0x40);
		for(i = 0; i < 4; i++)
			spi_send_recv(0);
		spi_send_recv(0x95);
	} while(sd_recv() != 1);
	
	spi_send_recv(0xFF);
	spi_send_recv(0xFF);
	spi_send_recv(0x48);
	spi_send_recv(0);
	spi_send_recv(0);
	spi_send_recv(0x01);
	spi_send_recv(0xA5);
	spi_send_recv(0x69);
	
	if(sd_recv() & 0x4) {
		type = SD_CARD_TYPE_SD;
		sd_send_command(55, 0);
		sd_recv();
		sd_send_command(41, 0);
		if(sd_recv() & 0x4)
			type = SD_CARD_TYPE_MMC;
	} else {
		type = SD_CARD_TYPE_SDHC;
		spi_send_recv(0xFF);
		spi_send_recv(0xFF);
		i=spi_send_recv(0xFF);
		if(!((i & 0x1) && (spi_send_recv(0xFF) == 0xA5)))
			return (type = SD_CARD_TYPE_INVALID);
	}
	i = 0;
	switch(type) {
		case SD_CARD_TYPE_MMC:
			do
				sd_send_command(1, 0);
			while(sd_recv());
			break;
		case SD_CARD_TYPE_SDHC:
			i = 0x40000000;
		case SD_CARD_TYPE_SD:
			do {
				sd_send_command(55, 0);
				sd_recv();
				sd_send_command(41, i);
			} while(sd_recv());
			
			sd_send_command(58, 0);
			sd_recv();
			if(!(spi_send_recv(0xFF) & 0x40))
				type = SD_CARD_TYPE_SD;
			for(i = 0; i < 4; i++)
				spi_send_recv(0xFF);
			break;
		default:
			break;
	}
	
	spi_set_clockdiv(1);
	dumbdelay(100000);
	
	sd_send_command(16, 512);
	sd_recv();
	
	spi_select_slave(SPI_SLAVE_NONE);
	spi_send_recv(0xff);
	
	return type;
}

void sd_send_command(uint8_t command, uint32_t arg) {
	uint8_t frame[6];
	
	frame[0] = ((command & 0x3F)) | 0x40;
	frame[1] = (arg >> 24);
	frame[2] = (arg >> 16);
	frame[3] = (arg >> 8);
	frame[4] = arg;
	frame[5] = 0x1;
	
	/*padding to let the card think*/
	spi_send_recv(0xFF);
	spi_send_recv(0xFF);
	
	spi_send_recv(frame[0]);
	spi_send_recv(frame[1]);
	spi_send_recv(frame[2]);
	spi_send_recv(frame[3]);
	spi_send_recv(frame[4]);
	spi_send_recv(frame[5]);
	
}

uint32_t sd_get_card_size(void) {
	/* Returns size in 512 byte sectors */
	uint32_t size = 0, blocksize;
	uint8_t ver, size_mult;
	int i;
	
	spi_select_slave(SPI_SLAVE_SD);
	
	sd_send_command(9, 0);
	sd_recv();
	sd_recv();
	
	ver=spi_send_recv(0xFF) >> 6;
	for(i = 0; i < 4; i++)
		spi_send_recv(0xFF);
	
	blocksize = 1 << (spi_send_recv(0xFF) & 0x0F);
	switch(ver) {
		case 0:
			size = ((uint16_t ) (spi_send_recv(0xFF) & 0x3))  << 10;
			size |= ((uint16_t ) spi_send_recv(0xFF)) << 2;
			size |= spi_send_recv(0xFF) >> 6;
			
			size_mult = (spi_send_recv(0xFF) & 0x3) << 1;
			size_mult |= spi_send_recv(0xFF) >> 7;
			size = ((size + 1)*(1 << (size_mult + 2))*blocksize)/512;
			break;
		case 1:
			spi_send_recv(0xFF);
			size = ((uint32_t) spi_send_recv(0xFF) & 0x3F) << 16;
			size |= ((uint32_t) spi_send_recv(0xFF)) << 8;
			size |= ((uint32_t) spi_send_recv(0xFF));
			spi_send_recv(0xFF);
			size *= 1024;
			break;
		default:
			for(i = 0; i < 5; i++)
				spi_send_recv(0xFF);
	}
	
	for(i = 0; i < 7; i++)
		spi_send_recv(0xFF);
	
	spi_select_slave(SPI_SLAVE_NONE);
	spi_send_recv(0xff);
	
	return size;
}

SDCardType sd_poll(void) {
	if(type == SD_CARD_TYPE_INVALID)
		return type;
	sd_send_command(13, 0);
	if(sd_recv() < 0)
		return (type = SD_CARD_TYPE_INVALID);
	spi_send_recv(0xFF);
	return type;
}

int16_t sd_recv(void) {
	uint8_t r;
	unsigned int tries = 0;
	while((r = spi_send_recv(0xff)) == 0xff)
		if(tries++ > 5000)
			return -1;
	return r;
}

uint8_t sd_stream_read_block(SDStreamStatus *status, ...) {
	/* First call - sd_stream_read_block(pointer to SD_STREAM_STATUS_BEGIN, sector number) */
	/* subsequent calls - sd_stream_read_block(pointer to SDStreamStatus) */
	/* Continue until status = SD_STREAM_STATUS_DONE */
	
	va_list va;
	uint8_t data;
	
	switch(*status) {
		case SD_STREAM_STATUS_BEGIN:
			spi_select_slave(SPI_SLAVE_SD);
			*status = SD_BLOCK_SIZE;
			data = 0;
			va_start(va, status);
			switch(type) {
				case SD_CARD_TYPE_MMC:
				case SD_CARD_TYPE_SD:
					data = 9;
				case SD_CARD_TYPE_SDHC:
					sd_send_command(17, va_arg(va, unsigned int) << data);
					data = sd_recv() < 0;
					data |= sd_recv() < 0;
					if(data)
						*status = SD_STREAM_STATUS_FAILED;
					break;
				case SD_CARD_TYPE_INVALID:
					*status = SD_STREAM_STATUS_FAILED;
					break;
			}
			va_end(va);
		case SD_STREAM_STATUS_DONE:
		case SD_STREAM_STATUS_FAILED:
			return 0;
		case 1:
			(*status)--;
			data = spi_send_recv(0xFF);
			/* read checksum */
			spi_send_recv(0xFF);
			spi_send_recv(0xFF);
			if(type == SD_CARD_TYPE_INVALID)
				*status = SD_STREAM_STATUS_FAILED;
			
			spi_select_slave(SPI_SLAVE_NONE);
			spi_send_recv(0xff);
			
			return data;
		default:
			(*status)--;
			data = spi_send_recv(0xFF);
			return data;
	}
}

#if 0
uint8_t sd_stream_read_multiple(SDStreamStatus *status, ...) {
	/* First call - sd_stream_read_block(pointer to SD_STREAM_STATUS_BEGIN, total sectors, sector number) */
	/* subsequent calls - sd_stream_read_block(pointer to SDStreamStatus) */
	/* Continue until status = SD_STREAM_STATUS_DONE */
	
	va_list va;
	uint8_t data;
	
	switch(*status) {
		case SD_STREAM_STATUS_BEGIN:
			va_start(va, status);
			*status = SD_BLOCK_SIZE * va_arg(va, unsigned int);
			data = 0;
			switch(type) {
				case SD_CARD_TYPE_MMC:
				case SD_CARD_TYPE_SD:
					data = 9;
				case SD_CARD_TYPE_SDHC:
					sd_send_command(17, va_arg(va, unsigned int) << data);
					data = sd_recv() < 0;
					data |= sd_recv() < 0;
					if(data)
						*status = SD_STREAM_STATUS_FAILED;
					break;
				case SD_CARD_TYPE_INVALID:
					*status = SD_STREAM_STATUS_FAILED;
					break;
			}
			va_end(va);
		case SD_STREAM_STATUS_DONE:
		case SD_STREAM_STATUS_FAILED:
			return 0;
		case 1:
			(*status)--;
			data = spi_send_recv(0xFF);
			/* read checksum */
			spi_send_recv(0xFF);
			spi_send_recv(0xFF);
			if(type == SD_CARD_TYPE_INVALID)
				*status = SD_STREAM_STATUS_FAILED;
			return data;
		default:
			switch(*status & 0x1FF) {
				case 0:
					/* data token on block boundary */
					spi_send_recv(0xFF);
				default:
					(*status)--;
					spi_send_recv(va_arg(va, unsigned int));
					break;
				case 1:
					(*status)--;
					spi_send_recv(va_arg(va, unsigned int));
					spi_send_recv(0xFF);
					spi_send_recv(0xFF);
			}
			(*status)--;
			data = spi_send_recv(0xFF);
			return data;
	}
}
#endif

void sd_stream_write_block(SDStreamStatus *status, ...) {
	/* First call - sd_stream_write_block(pointer to SD_STREAM_STATUS_BEGIN, sector number) */
	/* subsequent calls - sd_stream_write_block(pointer to SDStreamStatus, data byte) */
	/* Continue until status = SD_STREAM_STATUS_DONE */
	
	va_list va;
	uint8_t data;
	
	va_start(va, status);
	switch(*status) {
		case SD_STREAM_STATUS_BEGIN:
			spi_select_slave(SPI_SLAVE_SD);
			*status = SD_BLOCK_SIZE;
			data = 0;
			switch(type) {
				case SD_CARD_TYPE_MMC:
				case SD_CARD_TYPE_SD:
					data = 9;
				case SD_CARD_TYPE_SDHC:
					sd_send_command(24, va_arg(va, unsigned int) << data);
					
					if(sd_recv() >= 0) {
						spi_send_recv(0xFF);
						spi_send_recv(0xFE);
						break;
					}
				case SD_CARD_TYPE_INVALID:
					*status = SD_STREAM_STATUS_FAILED;
					break;
			}
		case SD_STREAM_STATUS_DONE:
		case SD_STREAM_STATUS_FAILED:
			break;
		case 1:
			(*status)--;
			spi_send_recv(va_arg(va, unsigned int));
			/* read checksum */
			spi_send_recv(0xFF);
			spi_send_recv(0xFF);
			sd_recv();
			while(spi_send_recv(0xFF) == 0);
			spi_send_recv(0xFF);
			if(type == SD_CARD_TYPE_INVALID)
				*status = SD_STREAM_STATUS_FAILED;
			
			spi_select_slave(SPI_SLAVE_NONE);
			spi_send_recv(0xff);
			
			break;
		default:
			(*status)--;
			spi_send_recv(va_arg(va, unsigned int));
			break;
	}
	va_end(va);
}

void sd_stream_write_multiple(SDStreamStatus *status, ...) {
	/* First call - sd_stream_read_block(pointer to SD_STREAM_STATUS_BEGIN, total sectors, first sector number) */
	/* subsequent calls - sd_stream_read_block(pointer to SDStreamStatus, data byte) */
	/* Continue until status < 1 */
	
	va_list va;
	uint8_t data;
	
	va_start(va, status);
	switch(*status) {
		case SD_STREAM_STATUS_BEGIN:
			spi_select_slave(SPI_SLAVE_SD);
			*status = SD_BLOCK_SIZE * va_arg(va, unsigned int);
			data = 0;
			switch(type) {
				case SD_CARD_TYPE_MMC:
				case SD_CARD_TYPE_SD:
					data = 9;
				case SD_CARD_TYPE_SDHC:
					sd_send_command(25, va_arg(va, unsigned int) << data);
					if(sd_recv() < 0)
						*status = SD_STREAM_STATUS_FAILED;
					break;
				case SD_CARD_TYPE_INVALID:
					*status = SD_STREAM_STATUS_FAILED;
					break;
			}
			
		case SD_STREAM_STATUS_DONE:
		case SD_STREAM_STATUS_FAILED:
			break;
		case 1:
			(*status)--;
			spi_send_recv(va_arg(va, unsigned int));
			/* send checksum */
			spi_send_recv(0xFF);
			spi_send_recv(0xFF);
			/* stop token */
			spi_send_recv(0xFD);
			sd_recv();
			while (!spi_send_recv(0xFF));
			if(type == SD_CARD_TYPE_INVALID)
				*status = SD_STREAM_STATUS_FAILED;
			
			spi_select_slave(SPI_SLAVE_NONE);
			spi_send_recv(0xff);
			
			break;
		default:
			switch(*status & 0x1FF) {
				case 0:
					/* data token on block boundary */
					spi_send_recv(0xFC);
				default:
					(*status)--;
					spi_send_recv(va_arg(va, unsigned int));
					break;
				case 1:
					(*status)--;
					spi_send_recv(va_arg(va, unsigned int));
					spi_send_recv(0xFF);
					spi_send_recv(0xFF);
					sd_recv();
					while (!spi_send_recv(0xFF));
			}
	}
	va_end(va);
}


SDCardType sd_get_type() {
	return type;
}
