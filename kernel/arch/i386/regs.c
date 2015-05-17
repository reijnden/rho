#include <kernel/regs.h>
#include <stdio.h>

void kdebug() {
	asm ("int $1" );
}

