
/*
 * Read an unsigned char from given IO channel
 */
unsigned char inportb(unsigned short _port) {
	unsigned char c;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (c) : "dN" (_port));
	return c;
}

/*
 * Write an unsigned char to given IO channel
 */
void outportb(unsigned short _port, unsigned char _data) {
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}
