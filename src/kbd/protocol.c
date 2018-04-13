#include "spi.h"
#include "protocol.h"
#include "keyboard.h"
#include "power.h"
#include "mouse.h"
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
	static uint8_t buf[4];
	static uint8_t len = 0;
	static uint8_t pos = 0;
	
	
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
		
		case PROTOCOL_COMMAND_MOUSE_EVENT:
			spi_send(0x00);
			mouse_vel_get(buf);
			len = 4;
			protocol.cmd = PROTOCOL_COMMAND_SEND_BUFFER;
			break;
			
		/* Emulate a digitizer */
		case PROTOCOL_COMMAND_DIGITIZER_EVENT:
			spi_send(0x00);
			mouse_state_get(buf);
			len = 4;
			protocol.cmd = PROTOCOL_COMMAND_SEND_BUFFER;
			break;
			
		case PROTOCOL_COMMAND_POWER_OFF:
			protocol.cmd = PROTOCOL_COMMAND_NONE;
			power_off();
			break;
		
		/* Special case for dumping buffers */
		case PROTOCOL_COMMAND_SEND_BUFFER:
			if(pos == len) {
				spi_send(0xFF);
				pos = 0;
				protocol.cmd = PROTOCOL_COMMAND_NONE;
			} else {
				spi_send(buf[pos++]);
			}
			
			break;
		
		default:
			spi_send(0xFF);
			protocol.cmd = PROTOCOL_COMMAND_NONE;
			break;
	}
}
