#include <stdio.h>
#include <stdlib.h>

#if defined(__is_rho_kernel)
#include <kernel/tty.h>
#endif

/*
 * putchar writes the character c, cast to an unsigned char, to stdout
 */
int putchar(int ic)
{
#if defined(__is_rho_kernel)
	char c = (unsigned char) ic;
	terminal_write(&c, sizeof(c));
#else
	// TODO: You need to implement a write system call.
	abort();
#endif
	return ic;
}
