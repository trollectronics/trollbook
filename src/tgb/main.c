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

StatusRegister reg_status;
ControlRegister reg_control;

ISR(TIMER0_COMPA_vect) {
	keyboard_tick();
	//power_tick();
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
