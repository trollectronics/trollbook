#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include <draw/font.h>

extern uint8_t fat_buf[512];
extern DrawFont *font_small;

void browse();
void player_init();
void play();

#endif
