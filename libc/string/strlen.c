#include <string.h>
#include <stdlib.h>

size_t strlen(const char* string)
{
#if defined(__is_rho_kernel)
	size_t result = 0;
	while ( string[result] )
		result++;
	return result;
#else
	//TODO
	abort();
	return -1;
#endif
}
