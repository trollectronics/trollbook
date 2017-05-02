#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <stdbool.h>

void interrupt_init(bool interrupt);
void interrupt_deinit();
void interrupt_assert();
void interrupt_deassert();

#endif
