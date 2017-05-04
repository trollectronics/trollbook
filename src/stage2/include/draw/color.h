#ifndef DRAW_COLOR_H_
#define DRAW_COLOR_H_

#include <stdint.h>

typedef uint8_t DrawColor;

extern DrawColor draw_color;

void draw_set_color(DrawColor col);

#endif
