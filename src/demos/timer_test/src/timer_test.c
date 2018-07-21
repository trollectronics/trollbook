#include <stdint.h>
#include <printf.h>
#include <terminal.h>


int main(int argc, char **argv) {
	int i, j;

	terminal_init();
	terminal_clear();

	delay_timer_set_prescale(30000);
	
	for (;;) {
		for (j = 0; j < 20; j++) {
			for (i = 0; i < 3; i++) {
				delay_timer(0, 250);
				printf(".");
			}
			
			delay_timer(3, 250);
			printf("#");
		}

		printf("\n");
	}
}
