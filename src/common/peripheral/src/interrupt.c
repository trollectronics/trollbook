#include <stdint.h>
#include "peripheral.h"
#include "interrupt.h"

static void **int_vector = (void *) 0xDE800;

static void (*handler[7])(int);

void interrupt_init() {
	int i;
	
	interrupt_move_vector((void *) 0xDE800);
	
	for(i = 0; i < 255; i++)
		int_vector[i] = interrupt_isr_unhandled;
	
	for(i = 25; i < 24 + 8; i ++)
		int_vector[i] = interrupt_isr_trampoline;
}

void interrupt_perihperal_enable(uint32_t n, uint8_t priority) {
	volatile uint32_t *interrupt_hw = (volatile uint32_t *) PERIPHERAL_INTERRUPT_BASE;
	interrupt_hw[32 + n] = 0x0;
	interrupt_hw[n] = priority & 0x7;
}

void interrupt_register(int priority, void (h)(int)) {
	handler[priority] = h;
}

void interrupt_isr_stage2(int number) {
	number -= 24;
	
	if(handler[number])
		handler[number](number);
}
