#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include "sleep.h"
#include "power.h"
#include "protocol.h"
#include "spi.h"
#include "keyboard.h"
#include "interrupt.h"

StatusRegister reg_status;
ControlRegister reg_control = {
	.keyboard_ie = true,
};

ISR(TIMER0_COMPA_vect) {
	keyboard_tick();
	interrupt_assert();
}

void timer_init() {
	TCCR0A = 0xD; //Clear on compare, clk/1024
	TCNT0 = 0;
	OCR0A = 24;
	TIMSK0 = 0x2; //Interrupt on compare A
	
}

void timer_deinit() {
	TCCR0A = 0x0;
	TIMSK0 = 0x0;
	TCNT0 = 0;
}

int main() {
	power_init();
	for(;;) {
		cli();
		sleep_enable();
		sei();
		sleep_cpu();
		sleep_disable();
	}
}
