#ifndef _DELAY_H_
#define _DELAY_H_
#include <stdint.h>

void dumbdelay(volatile uint32_t t);
void delay_timer_set_prescale(uint16_t prescaler);
void delay_timer(int timer, int32_t msec);

#endif
