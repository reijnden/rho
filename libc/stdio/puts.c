#include <stdio.h>
#include <stdlib.h>

/*
 * man puts:
 * puts writes the string s and a trailing newline to stdout
 */
int puts(const char *s)
{
	int i = 0;
#if defined(__is_rho_kernel)
	char *c = s;
	while (*c && putchar(*c) == *c) {
		c++;
		i++;
	}
	putchar(10);
#else
	//TODO
	abort();
#endif
	return i;
}
