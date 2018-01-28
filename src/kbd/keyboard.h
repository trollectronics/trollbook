#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdint.h>

uint8_t keyboard_init();
void keyboard_deinit();
void keyboard_event_push(uint8_t ev);
int16_t keyboard_event_pop();
uint8_t keyboard_events();
void keyboard_tick();

#endif
