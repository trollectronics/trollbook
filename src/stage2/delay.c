#include <stdint.h>
#include "delay.h"

void dumbdelay(uint32_t t) {
	while(t--);
}
