#include <stdint.h>
#include <stdbool.h>
#include "peripheral.h"
#include "printf.h"

#define BLOCK_SIZE 32768
#define NO_BLOCKS 1024
#define BLOCKS_TO_WRITE 1024
#define PRINT_DOT_MASK 0x1F

int memtest_run(bool use_cache) {
	int i, j, k;
	volatile uint16_t *ptr = (void *) SDRAM_BASE;
	
	if(use_cache) {
		__asm__ __volatile__ (
			"cinva %%bc\n"
			"move.l #0x80008000, %%d0\n"
			"movec %%d0, %%cacr\n"
			:
			:
			: "d0"
		);
	} else {
		__asm__ __volatile__ (
			"move.l #0x00000000, %%d0\n"
			"movec %%d0, %%cacr\n"
			:
			:
			: "d0"
		);
	}
	
	printf("\nTesting:\n");
	
	for (i = 0; i < NO_BLOCKS; i = (i?(i<<1):1)) {
		for (j = 0; j < BLOCK_SIZE; j++)
			ptr[i * BLOCK_SIZE + j] = j;
		for (j = 0; j < BLOCKS_TO_WRITE; j++) {
			if (j == i)
				continue;
			for (k = 0; k < BLOCK_SIZE; k++)
				ptr[j * BLOCK_SIZE + k] = 0xFFFF;
			
			if((j & PRINT_DOT_MASK) == PRINT_DOT_MASK)
				printf(".");
		}
		
		if(use_cache) {
			__asm__ __volatile__ ("cpusha %dc\n");
			__asm__ __volatile__ ("cinva %dc\n");
		}
		for (j = 0; j < BLOCK_SIZE; j++)
			if (ptr[i * BLOCK_SIZE + j] != j) {
				printf("\nFAIL at 0x%X\n", (uint32_t) &ptr[i * BLOCK_SIZE + j]);
				printf("Expected 0x%X, got 0x%X\n", j, ptr[i * BLOCK_SIZE + j]);
				
				return 0;
			}
		printf("\n");
	}

	printf("\nsuccess!\n");
	return 1;
}

