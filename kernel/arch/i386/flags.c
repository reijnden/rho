#include <stdint.h>

/*
 * Pop the flags register into a variable and return it.
 */
uint32_t eflags() {
	uint32_t eflags;
	__asm__ __volatile__ (
		"pushf \n"
		"popl %%eax \n"
		"movl %%eax, %0" : "=rm" (eflags) );
	return eflags;
}


