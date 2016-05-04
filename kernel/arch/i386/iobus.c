
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

/*
 * Read an unsigned long from given IO channel
 */
unsigned long inportl(unsigned short _port) {
	unsigned long l;
	__asm__ __volatile__ ("inl %1, %0" : "=a" (l) : "dN" (_port));
	return l;
}

/*
 * Write an unsigned long to given IO channel
 */
void outportl(unsigned short _port, unsigned long _data) {
	__asm__ __volatile__ ("outl %1, %0" : : "dN" (_port), "a" (_data));
}
