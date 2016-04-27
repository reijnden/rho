#include <string.h>
#include <stdlib.h>

void* memset(void* bufptr, int value, size_t size)
{
#if defined(__is_rho_kernel)
	unsigned char* buf = (unsigned char*) bufptr;
	for ( size_t i = 0; i < size; i++ )
		buf[i] = (unsigned char) value;
	return bufptr;
#else
	//TODO
	abort();
	return 0;
#endif
}
