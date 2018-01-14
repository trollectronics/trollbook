#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <stdint.h>

#define INTERRUPT_OFFSET_TRAP 32

void interrupt_isr_unhandled();
void interrupt_isr_trampoline();

void interrupt_init();
void interrupt_perihperal_enable(uint32_t n, uint8_t priority);
void interrupt_register(int priority, void (h)(int));
void interrupt_move_vector(void *addr);

void interrupt_global_enable();
void interrupt_global_disable();

#endif
