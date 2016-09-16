#include <stdint.h>
#include <avr/io.h>

//TODO: sleep is broken
void usleep(uint16_t us) {
	TCCR1A = 0x04; //reset on match mode
	OCR1A = us;
	TCNT1 = 0;
	TIFR1 |= 0x2;
	TCCR1B = 0x02; //internal clock, prescale by 8
	
	while(!(TIFR1 & 0x2));
	TCCR1B = 0x00;
	TIFR1 |= 0x2;
}

void msleep(uint16_t ms) {
	while(ms--)
		usleep(1000);
}
