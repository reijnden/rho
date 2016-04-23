#include <kernel/regs.h>
//#include <stdio.h>

/*
 * Software interrupt 1 = Debugger
 * The ISR handler will dump the contents of all registers to screen
 */
void kdebug() {
	asm ("int $1" );
}

uint32_t cr0() {
	uint32_t cr0;
	asm ("movl %%cr0,%%eax\n"
	     "movl %%eax, %0" : "=rm" (cr0) );
	return cr0;
}
