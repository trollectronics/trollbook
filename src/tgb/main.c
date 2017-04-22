#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "sleep.h"
#include "power.h"
#include "protocol.h"

StatusRegister reg_status;
ControlRegister reg_control;

int main() {
	power_init();
	for(;;) {
		power_tick();
		msleep(10);
	}
}
