#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <avr/io.h>
#include "protocol.h"
#include "interrupt.h"

#define KBD_RESET_DEASSERT() (PORTB |= (1 << 0))
#define KBD_RESET_ASSERT() (PORTB &= ~(1 << 0))

#define KBD_CLK_ASSERT() (PORTB |= (1 << 1))
#define KBD_CLK_DEASSERT() (PORTB &= ~(1 << 1))

#define KBD_Q0 ((PINA >> 2) & 1)
#define KBD_Q1 ((PIND >> 4) & 1)
#define KBD_Q2 ((PIND >> 3) & 1)
#define KBD_Q3 ((PIND >> 1) & 1)

#define COLUMNS 17

typedef enum KeyboardState KeyboardState;
enum KeyboardState {
	KEYBOARD_STATE_RESET,
	KEYBOARD_STATE_CLK_LOW,
	KEYBOARD_STATE_CLK_HIGH,
};

struct {
	uint8_t keystate[9];
	uint8_t column;
	KeyboardState state;
	uint8_t event[8];
	uint8_t events;
} keyboard;

void keyboard_init() {
	DDRB |= (1 << 0); //KBD_RESET output
	DDRB |= (1 << 1); //KBD_CLK output
	DDRA &= ~(1 << 2); //KBD_Q0 input
	DDRD &= ~(1 << 4); //KBD_Q1 input
	DDRD &= ~(1 << 3); //KBD_Q2 input
	DDRD &= ~(1 << 1); //KBD_Q3 input
	KBD_CLK_DEASSERT();
	KBD_RESET_ASSERT();
	
	memset(&keyboard, 0, sizeof(keyboard));
}

void keyboard_deinit() {
	DDRB &= ~(1 << 0); //KBD_RESET input
	DDRB &= ~(1 << 1); //KBD_CLK input
	DDRA &= ~(1 << 2); //KBD_Q0 input
	DDRD &= ~(1 << 4); //KBD_Q1 input
	DDRD &= ~(1 << 3); //KBD_Q2 input
	DDRD &= ~(1 << 1); //KBD_Q3 input
}

//TODO: make this circle buffer instead of stack
void keyboard_event_push(uint8_t ev) {
	reg_status.keyboard_if = true;
	if(keyboard.events == 7)
		return;
	
	keyboard.event[keyboard.events++] = ev;
}

int16_t keyboard_event_pop() {
	uint8_t ret;
	
	if(keyboard.events == 0) {
		reg_status.keyboard_if = false;
		return -1;
	}
	ret = keyboard.event[--keyboard.events];
	if(keyboard.events == 0)
		reg_status.keyboard_if = false;
	
	return ret;
}

uint8_t keyboard_events() {
	return keyboard.events;
}

void keyboard_tick() {
	uint8_t row;
	uint8_t oldrow;
	uint8_t i;
	
	switch(keyboard.state) {
		case KEYBOARD_STATE_RESET:
			KBD_CLK_DEASSERT();
			KBD_RESET_DEASSERT();
			keyboard.state = KEYBOARD_STATE_CLK_LOW;
			break;
		case KEYBOARD_STATE_CLK_LOW:
			KBD_CLK_ASSERT();
			keyboard.state = KEYBOARD_STATE_CLK_HIGH;
			break;
		case KEYBOARD_STATE_CLK_HIGH:
			row = KBD_Q0 | (KBD_Q1 << 1) | (KBD_Q2 << 2) | (KBD_Q3 << 3);
			if(keyboard.column & 1) {
				oldrow = keyboard.keystate[keyboard.column >> 1] >> 4;
				keyboard.keystate[keyboard.column >> 1] &= 0x0F;
				keyboard.keystate[keyboard.column >> 1] |= row << 4;
			} else {
				oldrow = keyboard.keystate[keyboard.column >> 1] & 0xF;
				keyboard.keystate[keyboard.column >> 1] &= 0xF0;
				keyboard.keystate[keyboard.column >> 1] |= row;
			}
			
			for(i = 0; oldrow != row; i++) {
				if((oldrow & 1) ^ (row & 1)) {
					keyboard_event_push(((oldrow & 1) << 7) | ((keyboard.column << 2) | i));
				}
				
				oldrow >>= 1;
				row >>= 1;
			}
			
			if(++keyboard.column == COLUMNS)
				keyboard.column = 0;
			KBD_CLK_DEASSERT();
			keyboard.state = KEYBOARD_STATE_CLK_LOW;
			break;
	}
}
