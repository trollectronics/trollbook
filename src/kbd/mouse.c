#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "protocol.h"
#include "mouse.h"

#define ABS(a) ((a) > 0 ? (a) : (-a))

struct {
	int16_t vel_x;
	int16_t vel_y;
	int16_t x;
	int16_t y;
	uint8_t chan; //0=Y, 1=X
	int16_t center_x;
	int16_t center_y;
} mouse;

void mouse_init() {
	uint16_t val;
	
	ADCSRA = 0x86; //ADC on, /64 prescaler
	DIDR0 |= (1 << 2) | (1 << 3); //Disable digital inputs
	mouse.chan = 0;
	
 	ADMUX = 0x42;
	ADCSRA |= 0x40; //Start conversion
	while(!(ADCSRA & 0x10));
	
	val = ADCL;
	val |= ((uint16_t) ADCH) << 8;
	/*Throw away first ADC value*/
	
	ADMUX = 0x42;
	ADCSRA |= 0x40; //Start conversion
	while(!(ADCSRA & 0x10));
	
	val = ADCL;
	val |= ((uint16_t) ADCH) << 8;
	
	mouse.center_x = val;
	
	ADMUX = 0x43;
	ADCSRA |= 0x40;
	while(!(ADCSRA & 0x10));
	
	val = ADCL;
	val |= ((uint16_t) ADCH) << 8;
	
	mouse.center_y = val;
	
	ADMUX = 0x42;
	ADCSRA |= 0x40;
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

void mouse_vel_get(uint8_t *buf) {
	reg_status.mouse_if = false;
	buf[0] = mouse.vel_x >> 8;
	buf[1] = mouse.vel_x;
	buf[2] = mouse.vel_y >> 8;
	buf[3] = mouse.vel_y;
}

void mouse_tick() {
	int16_t diff;
	if(ADCSRA & 0x10) {
		diff = ADCL;
		diff |= ((uint16_t) ADCH) << 8;
		
		if(mouse.chan == 1)
			diff -= mouse.center_y;
		else
			diff -= mouse.center_x;
		
		if(ABS(diff) < MOUSE_THRESHOLD)
			diff = 0;
		else {
			reg_status.mouse_if = true;
			
			if(diff < MOUSE_THRESHOLD)
				diff += MOUSE_THRESHOLD;
			else
				diff -= MOUSE_THRESHOLD;
		}
		
		if(mouse.chan == 0) {
			mouse.vel_x = -diff;
			mouse.x += diff/16;
			
			if(mouse.x < 0)
				mouse.x = 0;
			if(mouse.x >= 800)
				mouse.x = 799;
			
			ADMUX = 0x43;
			mouse.chan = 1;
		} else {
			mouse.vel_y = -diff;
			mouse.y += diff/16;
			
			if(mouse.y < 0)
				mouse.y = 0;
			if(mouse.y >= 480)
				mouse.y = 479;
			
			ADMUX = 0x42;
			mouse.chan = 0;
		}
		ADCSRA |= 0x40; //Start conversion
	}
}
