#include <stdint.h>
#include <stdbool.h>
#include "spi.h"
#include "input.h"

#define SCANCODE_RMB 65
#define SCANCODE_MMB 66
#define SCANCODE_LMB 67

typedef enum ProtocolCommand ProtocolCommand;
enum ProtocolCommand {
	PROTOCOL_COMMAND_SEND_BUFFER = -2,
	PROTOCOL_COMMAND_NONE = -1,
	PROTOCOL_COMMAND_STATUS,
	PROTOCOL_COMMAND_CONTROL,
	PROTOCOL_COMMAND_KEYBOARD_EVENT,
	PROTOCOL_COMMAND_MOUSE_EVENT,
	PROTOCOL_COMMAND_POWER_OFF,
	PROTOCOL_COMMAND_POWER_REBOOT,
	PROTOCOL_COMMAND_VOLUME,
	PROTOCOL_COMMAND_ADC,
	PROTOCOL_COMMAND_I2C,
	PROTOCOL_COMMAND_DIGITIZER_EVENT,
};

static const char matrix_key[4][17] = {
	{'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '\'', INPUT_KEY_BACKSPACE, INPUT_KEY_LTROLL, INPUT_KEY_LALT, INPUT_KEY_UP, INPUT_KEY_LCTRL, },
	{'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', '^', INPUT_KEY_RETURN, INPUT_KEY_RALT, '<', INPUT_KEY_RIGHT, INPUT_KEY_RMB, },
	{'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'o', 'a', '\'', INPUT_KEY_ESC, INPUT_KEY_TAB, '~', INPUT_KEY_DOWN, INPUT_KEY_MMB, },
	{INPUT_KEY_LSHIFT, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', INPUT_KEY_RSHIFT, INPUT_KEY_RFN, ' ', INPUT_KEY_LFN, INPUT_KEY_LEFT, INPUT_KEY_LMB, },
};

static const char matrix_key_shift[4][17] = {
	{'!', '"', '#', '$', '%', '&', '/', '(', ')', '=', '?', '`', INPUT_KEY_BACKSPACE, INPUT_KEY_LTROLL, INPUT_KEY_LALT, INPUT_KEY_UP, INPUT_KEY_LCTRL, },
	{'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', '^', INPUT_KEY_RETURN, INPUT_KEY_RALT, '>', INPUT_KEY_RIGHT, INPUT_KEY_RMB, },
	{'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'O', 'A', '*', INPUT_KEY_ESC, INPUT_KEY_TAB, '|', INPUT_KEY_DOWN, INPUT_KEY_MMB, },
	{INPUT_KEY_LSHIFT, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', INPUT_KEY_RSHIFT, INPUT_KEY_RFN, ' ', INPUT_KEY_LFN, INPUT_KEY_LEFT, INPUT_KEY_LMB, },
};

static bool matrix[4][17];
static const int matrix_cols[17] = {2, 1, 3, 6, 7, 0, 4, 5, 8, 12, 10, 14, 15, 11, 9, 13, 16};

static struct {
	uint8_t event[8];
	uint8_t event_first;
	uint8_t event_last;
	bool shift;
} keyboard;

static struct {
	int16_t x;
	int16_t y;
	struct {
		uint8_t lmb : 1;
		uint8_t mmb : 1;
		uint8_t rmb : 1;
	} buttons;
} mouse;

void input_keyboard_event_push(uint8_t ev) {
	uint8_t last;
	
	last = keyboard.event_last;
	keyboard.event[keyboard.event_last++] = ev;
	if(keyboard.event_last == 8)
		keyboard.event_last = 0;
	
	if(keyboard.event_last == keyboard.event_first)
		keyboard.event_last = last;
}

InputKeyboardEvent input_keyboard_event_pop() {
	InputKeyboardEvent event = {0};
	uint8_t keycode;
	
	if(keyboard.event_first == keyboard.event_last) {
		return event;
	}
	
	keycode = keyboard.event[keyboard.event_first++];
	if(keyboard.event_first == 8)
		keyboard.event_first = 0;
	
	event.type = (keycode & 0x80) ? INPUT_KEYBOARD_EVENT_TYPE_RELEASE : INPUT_KEYBOARD_EVENT_TYPE_PRESS;
	if(keyboard.shift)
		event.keysym = matrix_key_shift[keycode & 0x3][matrix_cols[(keycode & 0x7F) >> 2]];
	else
		event.keysym = matrix_key[keycode & 0x3][matrix_cols[(keycode & 0x7F) >> 2]];
	
	if(event.keysym == INPUT_KEY_LSHIFT || event.keysym == INPUT_KEY_RSHIFT) {
		keyboard.shift = !(keycode & 0x8);
	}
	
	return event;
}

InputMouseEvent input_mouse_get() {
	InputMouseEvent event;
	event.x = mouse.x;
	event.y = mouse.y;
	event.buttons = mouse.buttons.lmb | (mouse.buttons.mmb << 1) | (mouse.buttons.rmb << 2);
	event.wheel = 0;
	
	return event;
}

void input_poll() {
	int i, j;
	uint8_t reg, status;
	spi_set_clockdiv(165);
	
	spi_select_slave(SPI_SLAVE_KBD);
	dumbdelay(1000);
	
	spi_send_recv(PROTOCOL_COMMAND_STATUS);
	status = spi_send_recv(0xFF);
	if(status != 0xFF && status & 0x1) {
		spi_send_recv(PROTOCOL_COMMAND_KEYBOARD_EVENT);
		
		while(dumbdelay(100), (reg = spi_send_recv(0xFF)) != 0xFF) {
			if((reg & 0x7F) == SCANCODE_LMB)
				mouse.buttons.lmb = !(reg & 0x80);
			else if((reg & 0x7F) == SCANCODE_MMB)
				mouse.buttons.mmb = !(reg & 0x80);
			else if((reg & 0x7F) == SCANCODE_RMB)
				mouse.buttons.rmb = !(reg & 0x80);
			else
				input_keyboard_event_push(reg);
		}
	}
	
	spi_send_recv(0xFF);
	
	if(status != 0xFF && status & 0x40) {
		spi_send_recv(PROTOCOL_COMMAND_DIGITIZER_EVENT);
		//spi_send_recv(0xFF);
		
		mouse.x = ((uint16_t) spi_send_recv(PROTOCOL_COMMAND_DIGITIZER_EVENT)) << 8;
		mouse.x |= spi_send_recv(PROTOCOL_COMMAND_DIGITIZER_EVENT);
		mouse.y = ((uint16_t) spi_send_recv(PROTOCOL_COMMAND_DIGITIZER_EVENT)) << 8;
		mouse.y |= spi_send_recv(PROTOCOL_COMMAND_DIGITIZER_EVENT);
		
		spi_send_recv(0xFF);
		
	}
	spi_select_slave(SPI_SLAVE_NONE);
}
