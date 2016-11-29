#include <stdint.h>
#include "printf.h"

int memtest_run() {
	int i, j, k;
	uint16_t *ptr = (void *) 0x40000000;

	for (i = 0; i < 1024; i = (i?(i<<1):1)) {
		for (j = 0; j < 32768; j++)
			ptr[i * 32768 + j] = j;
		for (j = 0; j < 1024; j++) {
			if (j == i)
				continue;
			for (k = 0; k < 32768; k++)
				ptr[j * 32768 + k] = 0xFFFF;
		}
		
		/* TODO: Push cache */
		__asm__ __volatile__ ("cpusha %dc\n");
		for (j = 0; j < 32768; j++)
			if (ptr[i * 32768 + j] != j)
				return printf("\nFAIL at 0x%X\n", (uint32_t) &ptr[i * 32768 + j]), 0;
		printf(".");
		if ((i & 0x3F) == 0x3F)
			printf("\n");
	}

	printf("\nsuccess!\n");
	return 1;
}

