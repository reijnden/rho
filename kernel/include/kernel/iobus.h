#ifndef _KERNEL_IOBUS_H
#define _KERNEL_IOBUS_H

/*
 * The Progammable Interrupt Controller channels
 */
#define PIC_MASTER		0x20
#define PIC_SLAVE		0xA0
/*
 * The Programmable Interval Timer channels
 */
#define PIT_CH0PORT	0x40				/* Channel 0 bound to IRQ0 */
#define PIT_CH1PORT	0x41				/* Channel 1 system specific */
#define PIT_CH2PORT	0x42				/* Channel 2 system speaker */
#define PIT_CMDPORT	0x43
/*
 * The keyboard channels
 */
#define KB_DATAPORT	0x60

unsigned char inportb(unsigned short _port);
void outportb(unsigned short _port, unsigned char _data);

#endif
