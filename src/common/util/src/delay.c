#include <stdint.h>
#include "peripheral.h"
#include "delay.h"

void dumbdelay(volatile uint32_t t) {
	while(t--);
}


void delay_timer_set_prescale(uint16_t prescaler) {
	volatile uint32_t *timer_base = (volatile uint32_t *) PERIPHERAL_TIMER_BASE;
	
	timer_base[0] = (timer_base[0] & 0x3) | (prescaler << 16);
}


void delay_timer(int timer, int32_t msec) {
	int prescaler, iterations, i;
	volatile uint32_t *timer_base = (volatile uint32_t *) PERIPHERAL_TIMER_BASE;

	msec = msec * 100000 / 3;
	prescaler = timer_base[0] >> 16;
	msec /= prescaler;
	iterations = msec >> 16;
	timer_base[0] = timer_base[0] & (~(1 << timer));

	for (i = 0; i < iterations; i++) {
		// Turn off the timer
		timer_base[4 * (timer + 1)] = 0;
		// Clear interrupt flag
		timer_base[1] = (1 << timer);
		
		timer_base[4 * (timer + 1) + 2] = 65535;
		
		timer_base[4 * (timer + 1)] = 1;

		while (!(timer_base[1] & (1 << timer)));
	}

	timer_base[4 * (timer + 1)] = 0;
	timer_base[1] = timer_base[1] & (1 << timer);
	timer_base[4 * (timer + 1) + 2] = msec & 0xFFFF;
	timer_base[4 * (timer + 1)] = 1;
	
	while (!(timer_base[1] & (1 << timer)));
	timer_base[4 * (timer + 1)] = 0;

	return;
}
