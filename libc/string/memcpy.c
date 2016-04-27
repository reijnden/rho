#include <string.h>
#include <stdlib.h>

void* memcpy(void* __restrict dstptr, const void* __restrict srcptr, size_t size)
{
#if defined(__is_rho_kernel)
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for ( size_t i = 0; i < size; i++ )
		dst[i] = src[i];
	return dstptr;
#else
	//TODO
	abort();
	return 0;
#endif
}
