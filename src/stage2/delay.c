#include <stdint.h>
#include "delay.h"

void dumbdelay(volatile uint32_t t) {
	while(t--);
}
