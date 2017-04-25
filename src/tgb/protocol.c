#include "spi.h"
#include "protocol.h"
#include "keyboard.h"
#include "power.h"
#include "interrupt.h"

struct {
	ProtocolCommand cmd;
} protocol = {
	PROTOCOL_COMMAND_NONE,
};

void protocol_reset() {
	protocol.cmd = PROTOCOL_COMMAND_NONE;
}

void protocol_tick() {
	int16_t dat;
	if(protocol.cmd == PROTOCOL_COMMAND_NONE)
		protocol.cmd = spi_recv();
	
	switch(protocol.cmd) {
		case PROTOCOL_COMMAND_STATUS:
			spi_send(*((uint8_t*) &reg_status));
			protocol.cmd = PROTOCOL_COMMAND_NONE;
			interrupt_deassert();
			break;
		
		case PROTOCOL_COMMAND_CONTROL:
			spi_send(*((uint8_t*) &reg_control));
			protocol.cmd = PROTOCOL_COMMAND_NONE;
			break;
		
		case PROTOCOL_COMMAND_KEYBOARD_EVENT:
			dat = keyboard_event_pop();
			if(dat < 0) {
				protocol.cmd = PROTOCOL_COMMAND_NONE;
				spi_send(0xFF);
			} else {
				spi_send(dat);
			}
			break;
		
		case PROTOCOL_COMMAND_POWER_OFF:
			protocol.cmd = PROTOCOL_COMMAND_NONE;
			power_off();
			break;
			
		default:
			spi_send(0xFF);
			break;
	}
}
