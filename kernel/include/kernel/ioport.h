#ifndef _KERNEL_IOPORT_H
#define _KERNEL_IOPORT_H

unsigned char inportb(unsigned short _port);
void outportb(unsigned short _port, unsigned char _data);

#endif
