#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void abort(void)
{
#if defined(__is_bizim_kernel)
	// TODO: Add proper kernel panic.
	printf("Kernel Panic: abort()\n");
#else
	printf("abort()\n");
#endif
	while ( 1 ) { }
	__builtin_unreachable();
}
