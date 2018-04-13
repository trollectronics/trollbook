#ifndef INPUT_H_
#define INPUT_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum InputKey InputKey;
enum InputKey {
	INPUT_KEY_NONE = 0,
	
	INPUT_KEY_BACKSPACE = 8,
	INPUT_KEY_TAB = 9,
	INPUT_KEY_RETURN = 13,
	INPUT_KEY_ESC = 27,
	INPUT_KEY_SPACE = 32,
	
	INPUT_KEY_LMB = 0x80,
	INPUT_KEY_MMB,
	INPUT_KEY_RMB,
	
	INPUT_KEY_UP,
	INPUT_KEY_DOWN,
	INPUT_KEY_LEFT,
	INPUT_KEY_RIGHT,
	
	INPUT_KEY_LCTRL,
	INPUT_KEY_LALT,
	INPUT_KEY_LTROLL,
	INPUT_KEY_LSHIFT,
	INPUT_KEY_LFN,
	
	INPUT_KEY_RCTRL,
	INPUT_KEY_RALT,
	INPUT_KEY_RTROLL,
	INPUT_KEY_RSHIFT,
	INPUT_KEY_RFN,
	
	INPUT_KEY_KP_ENTER
};

typedef enum InputKeyboardEventType InputKeyboardEventType;
enum InputKeyboardEventType {
	INPUT_KEYBOARD_EVENT_TYPE_NONE,
	INPUT_KEYBOARD_EVENT_TYPE_PRESS,
	INPUT_KEYBOARD_EVENT_TYPE_RELEASE,
};

typedef struct InputKeyboardEvent InputKeyboardEvent;
struct InputKeyboardEvent {
	InputKeyboardEventType type;
	InputKey keysym;
	//char unicode[5];
};

typedef struct InputMouseEvent InputMouseEvent;
struct InputMouseEvent {
	int16_t x;
	int16_t y;
	uint8_t buttons;
	int8_t wheel;
};


void input_keyboard_event_push(uint8_t ev);
InputKeyboardEvent input_keyboard_event_pop();
InputMouseEvent input_mouse_get();
void input_poll();


#endif
