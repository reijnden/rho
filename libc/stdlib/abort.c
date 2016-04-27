#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void abort(void)
{
#if defined(__is_rho_kernel)
	// TODO: Add proper kernel panic.
	puts("Kernel Panic: abort()");
#else
	puts("abort()");
#endif
	while ( 1 ) { }
	__builtin_unreachable();
}
