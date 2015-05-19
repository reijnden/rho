#include <kernel/regs.h>
#include <stdio.h>

/*
 * Software interrupt 1 = Debugger
 * The ISR handler will dump the contents of all registers to screen
 */
void kdebug() {
	asm ("int $1" );
}

