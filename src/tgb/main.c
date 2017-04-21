#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "sleep.h"

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

void power_off() {
	PWRON_RESET_ASSERT();
	msleep(100);
	PWR_SHUTDOWN_24_ASSERT();
	msleep(50);
	PWR_SHUTDOWN_3_ASSERT();
	
	power_state = POWER_STATE_OFF;
}

void power_on() {
	PWR_SHUTDOWN_3_DEASSERT();
	msleep(100);
	PWR_SHUTDOWN_24_DEASSERT();
	msleep(500);
	PWRON_RESET_DEASSERT();
	
	power_state = POWER_STATE_ON;
}

int main() {
	PWRON_RESET_ASSERT();
	PWR_SHUTDOWN_24_ASSERT();
	PWR_SHUTDOWN_3_ASSERT();
	DDRD &= ~(1 << 2); //Power button input
	DDRD |= (1 << 6); //PWR_SHUTDOWN_24 output
	DDRD |= (1 << 7); //PWR_SHUTDOWN_3 output
	DDRB |= (1 << 7); //PWRON_RESET output
	
	bool power_button = 1, power_button_old = 1;
	
	for(;;) {
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
		
		msleep(10);
	}
}
