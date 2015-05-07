#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/*
 * printf uses putchar do do the actual writing
 */
static void print(const char* data, size_t data_length)
{
	for ( size_t i = 0; i < data_length; i++ )
		putchar((int) ((const unsigned char*) data)[i]);
}

static void printllbase (unsigned long long l, int base) {
	static char tab[16] = "0123456789abcdef";
	if (l / base != 0)
		printllbase(l / base, base);
	print (&tab[l % base],sizeof(tab[l % base]));
}

static void printlbase (unsigned long l, int base) {
	static char tab[16] = "0123456789abcdef";
	if (l / base != 0)
		printlbase(l / base, base);
	print (&tab[l % base],sizeof(tab[l % base]));
}

static void printbase (unsigned int i, int base) {
	static char tab[16] = "0123456789abcdef";
	if (i / base != 0)
		printbase(i / base, base);
	print (&tab[i % base],sizeof(tab[i % base]));
}

static void printi (int i) {
	if (i / 10 != 0)
		printi(i / 10);
	char c = (char)(i % 10 + 48);
	print (&c,sizeof(c));
}

int printf(const char* __restrict format, ...)
{
	va_list parameters;
	va_start(parameters, format);

	int written = 0;
	char mod1 = 0;
	char mod2 = 0;
	size_t amount;
	bool rejected_bad_specifier = false;

	while ( *format != '\0' )
	{
		if ( *format != '%' )
		{
			/*
			 * No conversion specifier
			 */
		print_c:
			amount = 1;
			while ( format[amount] && format[amount] != '%' )
				amount++;
			print(format, amount);
			format += amount;
			written += amount;
			continue;
		}
		/*
		 * '%' character found
		 */
		const char* format_begun_at = format;

		/*
		 * '%' followed by '%'.
		 */
		if ( *(++format) == '%' )
			goto print_c;

	formatmatch:
		if ( rejected_bad_specifier )
		{
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}
		else if ( *format == 'c' )
		{
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			print(&c, sizeof(c));
		}
		else if ( ( *format == 'd' ) || ( *format == 'i' ) )
		{
			format++;
			int i = va_arg(parameters, int);
			if (i<0) {
				i = -i;
				print ("-",strlen("-"));
			}
			printi(i);
		}
		else if ( *format == 'l' )
		{
			format++;
			if (!mod1)
				mod1 = 'l';
			else
			       	mod2 = 'l';
			goto formatmatch;
		}
		else if ( *format == 'o' )
		{
			format++;
			unsigned int o = va_arg(parameters, unsigned int);
			printbase(o, 8);
		}
		else if ( *format == 's' )
		{
			format++;
			const char* s = va_arg(parameters, const char*);
			print(s, strlen(s));
		}
		else if ( *format == 'u' )
		{
			format++;
			if (mod2 == 'l') {
				unsigned long long u = va_arg(parameters, unsigned long long);
				printllbase(u, 10);
				mod1 = mod2 = 0;
			}
			else if (mod1 == 'l') {
				unsigned long u = va_arg(parameters, unsigned long);
				printlbase(u, 10);
				mod1 = 0;
			} else {
				unsigned int u = va_arg(parameters, unsigned int);
				printbase(u, 10);
			}
		}
		else if ( *format == 'x' )
		{
			format++;
			if (mod2 == 'l') {
				unsigned long long x = va_arg(parameters, unsigned long long);
				printllbase(x, 16);
				mod1 = mod2 = 0;
			}
			else if (mod1 == 'l') {
				unsigned long x = va_arg(parameters, unsigned long);
				printlbase(x, 16);
				mod1 = 0;
			} else {
				unsigned int x = va_arg(parameters, unsigned int);
				printbase(x, 16);
			}
		}
		else
		{
			/*
			 * Unknown conversion specifier
			 */
			goto incomprehensible_conversion;
		}
	}

	va_end(parameters);

	return written;
}
