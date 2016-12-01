#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdint.h>

typedef struct InputButtons InputButtons;
struct InputButtons {
	uint32_t left : 1;
	uint32_t right : 1;
	uint32_t up : 1;
	uint32_t down : 1;
	
	uint32_t enter : 1;
	uint32_t back : 1;
};

InputButtons input_poll_uart();
InputButtons input_poll_temp_spi();

#define input_poll input_poll_temp_spi

#endif
