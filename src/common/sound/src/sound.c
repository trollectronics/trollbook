#include <stdint.h>
#include <stdbool.h>
#include "peripheral.h"

void sound_setup(void *buf_addr) {
	int i;
	volatile uint16_t *buf= (volatile uint16_t *) buf_addr;
	volatile uint32_t *sound_hw = (volatile uint32_t *) PERIPHERAL_SOUND_BASE;
	
	for(i = 0; i < 1024; i++) {
		buf[i] = 0;
	}
	
	sound_hw[0] = 0x0;
	sound_hw[2] = (uint32_t) buf_addr;
	sound_hw[3] = 3;
	sound_hw[4] = 512;
}

void sound_start() {
	volatile uint32_t *sound_hw = (volatile uint32_t *) PERIPHERAL_SOUND_BASE;
	volatile uint32_t *interrupt_hw = (volatile uint32_t *) PERIPHERAL_INTERRUPT_BASE;
	interrupt_hw[32 + 11] = 0x0;
	sound_hw[0] = 0x1;
}

void sound_stop() {
	volatile uint32_t *sound_hw = (volatile uint32_t *) PERIPHERAL_SOUND_BASE;
	volatile uint32_t *interrupt_hw = (volatile uint32_t *) PERIPHERAL_INTERRUPT_BASE;
	sound_hw[0] = 0x0;
	interrupt_hw[32 + 11] = 0x0;
}

int sound_wait() {
	volatile uint32_t *sound_hw = (volatile uint32_t *) PERIPHERAL_SOUND_BASE;
	volatile uint32_t *interrupt_hw = (volatile uint32_t *) PERIPHERAL_INTERRUPT_BASE;
	
	while(!interrupt_hw[32 + 11]);
	interrupt_hw[32 + 11] = 0x0;
	
	return !(sound_hw[1] & 0x1);
}
