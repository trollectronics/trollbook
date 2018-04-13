#ifndef MOUSE_H_
#define MOUSE_H_

#define MOUSE_THRESHOLD 32

#include <stdint.h>
#include <avr/io.h>
#include "mouse.h"

#define ABS(a) ((a) > 0 ? (a) : (-a))

void mouse_init();
void mouse_deinit();
void mouse_state_get(uint8_t *buf);
void mouse_vel_get(uint8_t *buf);
void mouse_tick();

#endif
