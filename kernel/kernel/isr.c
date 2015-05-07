#include <kernel/tty.h>
#include <stdio.h>

void interrupt_handler(void) {
	printf("Interrupt!\n");
	return;
}
