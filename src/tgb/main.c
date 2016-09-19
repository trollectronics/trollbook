#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "sleep.h"

#define ABS(a) ((a) > 0 ? (a) : (-a))

int main() {
	PORTB = 0x0;
	//PORTA = 0x0;
	DDRA = 0xF0;
	
	DIDR0 = 0x3;
	ADCSRA = 0x83;
	ADCSRB = 0x10;
	ADCSRA |= 0x40;
	
	sei();
	
	for(;;) {
	}
	
	return 0;
}