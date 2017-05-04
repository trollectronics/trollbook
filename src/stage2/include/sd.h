#ifndef __SD_H__

#define SD_BLOCK_SIZE 512

typedef enum {
	SD_CARD_TYPE_INVALID = -1,
	SD_CARD_TYPE_MMC,
	SD_CARD_TYPE_SD,
	SD_CARD_TYPE_SDHC,
} SDCardType;

enum {
	SD_STREAM_STATUS_FAILED= -2,
	SD_STREAM_STATUS_BEGIN = -1,
	SD_STREAM_STATUS_DONE,
};

typedef int SDStreamStatus;

SDCardType sd_init(void);
void sd_send_command(uint8_t command, uint32_t arg);
SDCardType sd_poll(void);
uint32_t sd_get_card_size(void);
int16_t sd_recv(void);
uint8_t sd_stream_read_block(SDStreamStatus *status, ...);
void sd_stream_write_block(SDStreamStatus *status, ...);
void sd_stream_write_multiple(SDStreamStatus *status, ...);
SDCardType sd_get_type();
#endif
