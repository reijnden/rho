
#include <kernel/iobus.h>
#include <kernel/irq.h>
#include <kernel/regs.h>
#include <stdio.h>

#define PIT_CMD		0x36				/* 00 11 011 0
							 * bit 0 	BCD		0 16 bit value
							 * bit 1-3 	Mode		3 Square Wave Mode
							 * bit 4 5 	RW mode		3 Low byte high byte
							 * bit 6 7 	Cntr		0 Channel select
							 */

#define HW_HERTZ	1193182				/* Don't change! Internal clockrate of the PIT */

static int _clockrate = 18;				/* If no hz is given we keep the default */
static unsigned long timer_ticks;			/* Our uptime counter */

/*
 * The PIT is counting down from divisor to zero.
 * When it hist zero it sends IRQ0 and resets.
 * Counting speed is HW_HERTZ. 
 * Example:
 * hz = HW_HERTZ: divisor = 1: 1193182 IRQs/second
 * hz = 100: divisor = 11931: 100 IRQs/second
 * divisor = 
 */
static void timer_phase(int hz) {
	int divisor = HW_HERTZ / hz;			/* result is hz ticks/second */
	outportb(PIT_CMDPORT, PIT_CMD);		
	outportb(PIT_CH0PORT, divisor & 0xFF);		/* low and high byte of divisor */
	outportb(PIT_CH0PORT, divisor >> 8);
}

/*
 * Our IRQ handler
 */
void timer_handler (struct regs *r) {
	timer_ticks++;
	if (timer_ticks % _clockrate == 0)		/* one second has passed */
		;
}

/*
 * Set up clock rate and install irq_handler for timer
 */
void timer_install(int clockrate) {
	if (clockrate > 0)
		_clockrate = clockrate;
	timer_phase(_clockrate);
	irq_install_handler(0, timer_handler);
}
