#include <string.h>
#include <stdlib.h>

int memcmp(const void* aptr, const void* bptr, size_t size)
{
#if defined(__is_rho_kernel)
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	for ( size_t i = 0; i < size; i++ )
		if ( a[i] < b[i] )
			return -1;
		else if ( b[i] < a[i] )
			return 1;
	return 0;
#else
	// TODO
	abort();
	return 0;
#endif
}
