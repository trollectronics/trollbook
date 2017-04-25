#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "interrupt.h"
#include "protocol.h"

void interrupt_init() {
	//DDRA &= ~(1 << 3);
	DDRA |= (1 << 3);
	PORTA |= (1 << 3);
	
	*((uint8_t *) &reg_status) = 0x0;
	*((uint8_t *) &reg_control) = 0x0;
	reg_control.keyboard_ie = true;
}

void interrupt_deinit() {
	DDRA &= ~(1 << 3);
	interrupt_deassert();
}

void interrupt_assert() {
	register uint8_t flag, mask;
	
	flag = *((uint8_t *) &reg_status);
	mask = *((uint8_t *) &reg_control);
	
	if(flag & mask) {
		//PORTA &= ~(1 << 3);
		//DDRA |= (1 << 3);
		//PORTA &= ~(1 << 3);
		PORTA &= ~(1 << 3);
	}
}

void interrupt_deassert() {
	//DDRA &= ~(1 << 3);
	PORTA |= (1 << 3);
}
