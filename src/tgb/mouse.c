#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "protocol.h"
#include "mouse.h"

#define ABS(a) ((a) > 0 ? (a) : (-a))

struct {
	uint16_t x;
	uint16_t y;
	uint8_t chan; //0=Y, 1=X
} mouse;

void mouse_init() {
	ADCSRA = 0x86; //ADC on, /64 prescaler
	DIDR0 |= (1 << 2) | (1 << 3); //Disable digital inputs
	mouse.chan = 0;
	
	ADMUX = 2;
	ADCSRA |= 0x40; //Start conversion
}

void mouse_deinit() {
	ADCSRA= 0x0;
}

void mouse_state_get(uint8_t *buf) {
	reg_status.mouse_if = false;
	buf[0] = mouse.x >> 8;
	buf[1] = mouse.x;
	buf[2] = mouse.y >> 8;
	buf[3] = mouse.y;
}

void mouse_tick() {
	int16_t diff;
	if(ADCSRA & 0x10) {
		diff = ADCL;
		diff |= ((uint16_t) ADCH) << 8;
		
		diff -= 512;
		if(ABS(diff) < MOUSE_THRESHOLD)
			diff = 0;
		else
			reg_status.mouse_if = true;
		
		if(mouse.chan == 0) {
			mouse.y += diff;
			
			if(mouse.y < 0)
				mouse.y = 0;
			if(mouse.y >= 800)
				mouse.y = 799;
			
			ADMUX = 3;
			mouse.chan = 1;
		} else {
			mouse.x += diff;
			
			if(mouse.x < 0)
				mouse.x = 0;
			if(mouse.x >= 480)
				mouse.x = 479;
			
			ADMUX = 2;
			mouse.chan = 0;
		}
		ADCSRA |= 0x40; //Start conversion
	}
}
