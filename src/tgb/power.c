#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include "sleep.h"
#include "power.h"
#include "spi.h"
#include "protocol.h"
#include "keyboard.h"
#include "interrupt.h"
#include "main.h"

#define POWER_BUTTON_KILL_TIMER 3000

#define PWRON_RESET_DEASSERT() (PORTB |= (1 << 7))
#define PWR_SHUTDOWN_24_DEASSERT() (PORTD |= (1 << 6))
#define PWR_SHUTDOWN_3_ASSERT() (PORTD |= (1 << 7))

#define PWRON_RESET_ASSERT() (PORTB &= ~(1 << 7))
#define PWR_SHUTDOWN_24_ASSERT() (PORTD &= ~(1 << 6))
#define PWR_SHUTDOWN_3_DEASSERT() (PORTD &= ~(1 << 7))

typedef enum PowerState PowerState;
enum PowerState {
	POWER_STATE_OFF,
	POWER_STATE_ON,
};

PowerState power_state = POWER_STATE_OFF;

ISR(INT0_vect) {
	uint16_t counter;
	for(counter = 0; !((PIND >> 2) & 1); counter++) {
		msleep(1);
		
		if(counter == POWER_BUTTON_KILL_TIMER && power_state == POWER_STATE_ON) {
			power_off();
		}
	}
	msleep(1);
	
	if(counter >= POWER_BUTTON_KILL_TIMER)
		return;
	
	switch(power_state) {
		case POWER_STATE_OFF:
			power_on();
			break;
		case POWER_STATE_ON:
			reg_status.powerbutton_if = 1;
			break;
	}
}

void power_init() {
	PWRON_RESET_ASSERT();
	PWR_SHUTDOWN_24_ASSERT();
	PWR_SHUTDOWN_3_ASSERT();
	DDRD &= ~(1 << 2); //Power button input
	DDRD |= (1 << 6); //PWR_SHUTDOWN_24 output
	DDRD |= (1 << 7); //PWR_SHUTDOWN_3 output
	DDRB |= (1 << 7); //PWRON_RESET output
	
	EICRA = 0x0; //Low level on pin is interrupt
	EIMSK = 0x1; //Enable interrupt for power button
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void power_off() {
	interrupt_deinit();
	timer_deinit();
	keyboard_deinit();
	spi_deinit();
	protocol_reset();
	PWRON_RESET_ASSERT();
	msleep(100);
	PWR_SHUTDOWN_24_ASSERT();
	msleep(50);
	PWR_SHUTDOWN_3_ASSERT();
	
	power_state = POWER_STATE_OFF;
	
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void power_on() {
	uint8_t key;
	
	PWR_SHUTDOWN_3_DEASSERT();
	msleep(100);
	PWR_SHUTDOWN_24_DEASSERT();
	
	power_state = POWER_STATE_ON;
	key = keyboard_init();
	interrupt_init(!!(key & 0x8));
	
	set_sleep_mode(SLEEP_MODE_IDLE);
	msleep(500);
	PWRON_RESET_DEASSERT();
	spi_init();
	timer_init();
}

bool power_button = 1, power_button_old = 1;

void power_tick() {
	if(!power_button_old && power_button) {
		switch(power_state) {
			case POWER_STATE_OFF:
				power_on();
				
				break;
			case POWER_STATE_ON:
				power_off();
				
				break;
		}
	}
	
	power_button_old = power_button;
	power_button = (PIND >> 2) & 1;
}

