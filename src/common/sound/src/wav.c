#include <stdint.h>
#include <stdbool.h>
#include "peripheral.h"
#include "sound.h"

#define HEADER_TEST(data, string) (data[0] == string[0] && data[1] == string[1] && data[2] == string[2] && data[3] == string[3])

#define PARSE_8(data) ((uint32_t) (data[0]))
#define PARSE_16(data) ((uint32_t) (data[0] | (data[1] << 8)))
#define PARSE_32(data) ((uint32_t) (data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24)))

void wav_play(void *data) {
	uint8_t *wav_data = data;
	uint8_t *wav_end;
	
	uint16_t sample;
	int i, j;
	uint32_t chunk_size;
	uint32_t total_size;
	uint32_t tmp;
	bool skip;
	
	static volatile uint16_t *sound_buffer[] = {
		(volatile uint16_t *) LLRAM_BASE,
		((volatile uint16_t *) LLRAM_BASE) + 512,
	};
	
	int buffer = 0;
	
	if(!HEADER_TEST(wav_data, "RIFF")) {
		//printf("Missing RIFF header\n");
		goto parsefail;
	}
	wav_data += 4;
	
	total_size = PARSE_32(wav_data);
	wav_data += 4;
	wav_end = wav_data  + total_size;
	
	if(!HEADER_TEST(wav_data, "WAVE")) {
		//printf("Missing WAVE header\n");
		goto parsefail;
	}
	wav_data += 4;
	
	if(!HEADER_TEST(wav_data, "fmt ")) {
		//printf("Missing fmt  header\n");
		goto parsefail;
	}
	wav_data += 4;
	
	chunk_size = PARSE_32(wav_data);
	wav_data += 4;
	data = wav_data + chunk_size;
	
	if(PARSE_16(wav_data) != 0x1) {
		//printf("Not a PCM file\n");
		goto fail;
	}
	wav_data += 2;
	
	if((tmp = PARSE_16(wav_data)) != 2) {
		//printf("Must have 2 channels (has %u)\n", tmp);
		goto fail;
	}
	wav_data += 2;
	
	if((tmp = PARSE_32(wav_data)) != 48000) {
		//printf("Must be 48 kHz (is %u kHz)\n", tmp/1000);
		goto fail;
	}
	wav_data += 4;
	
	wav_data += 6;
	
	if((tmp = PARSE_16(wav_data)) != 8) {
		//printf("Must be 8 bits per sample (is %u)\n", tmp);
		goto fail;
	}
	wav_data += 2;
	
	//printf("Found conformant WAV file, will play\n");
	
	while(data < (void *) wav_end) {
		wav_data = data;
		skip = !HEADER_TEST(wav_data, "data");
		wav_data += 4;
		chunk_size = PARSE_32(wav_data);
		wav_data += 4;
		data = wav_data + chunk_size;
		if(skip)
			continue;
		
		sound_setup((void *) LLRAM_BASE);
		i = 0;
		for(buffer = 0; buffer < 2; buffer++) {
			for(j = 0; j < 512; j++, i++) {
				if(i >= (chunk_size >> 1)) {
					sample = 0;
				} else {
					sample = *wav_data++;
					sample |= ((*wav_data++) << 8);
				}
				
				sound_buffer[buffer][j] = sample;
			}
		}
		
		//printf("Starting playback\n");
		sound_start();
		
		while(i < (chunk_size >> 1)) {
			buffer = sound_wait();
			for(j = 0; j < 512; j++, i++) {
				if(i >= (chunk_size >> 1)) {
					sample = 0;
				} else {
					sample = *wav_data++;
					sample |= ((*wav_data++) << 8);
				}
				sound_buffer[buffer][j] = sample;
			}
		}
	}
	
	sound_stop();
	return;
	
	parsefail:
	//printf("Failed to parse wave file\n");
	
	fail:
	return;
}
