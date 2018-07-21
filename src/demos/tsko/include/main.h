#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>

extern uint8_t fat_buf[512];

typedef struct DemoData DemoData;
struct DemoData {
	uint8_t a1[64000];
	uint8_t a2[64000];
	uint8_t am[64000];
	uint8_t and[64000];
	uint8_t b1[64000];
	uint8_t b2[64000];
	uint8_t bej[64000];
	uint8_t c1[64000];
	uint8_t c2[64000];
	uint8_t c3[64000];
	uint8_t c4[64000];
	uint8_t c5[64000];
	uint8_t find[64000];
	uint8_t h3y[64000];
	uint8_t him[64000];
	uint8_t jader[64000];
	uint8_t know[64000];
	uint8_t kraft[64000];
	uint8_t me[64000];
	uint8_t mist[64000];
	uint8_t mlarge[64000];
	uint8_t msmall[64000];
	uint8_t terror[64000];
	uint8_t tj0[64000];
	uint8_t tsko[64000];
	uint8_t want[64000];
	uint8_t you[64000];
	
	
	void *modfile;
	uint32_t modsize;
};

extern DemoData data;

void demo();

#endif
