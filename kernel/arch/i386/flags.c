#include <stdint.h>

uint32_t eflags() {
	uint32_t eflags = 0;
	__asm__ __volatile__ (
		"pushf \n"
		"popl %%eax \n"
		"movl %%eax, %0" : "=rm" (eflags) );
	return eflags;
}


