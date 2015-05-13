
#include <kernel/ioport.h>
#include <kernel/irq.h>
#include <kernel/regs.h>
#include <stdio.h>

#define CLOCKRATE	1193180		/* Don't change this value. It is the internal clockrate of the PIT */
static int _clockrate = CLOCKRATE;	/* Default value, 1 tick per second ;-)	*/
static int timer_ticks;			/* Our uptime counter			*/

static void timer_phase(int hz) {
	int divisor = CLOCKRATE / hz;	/* result is hz ticks/second		*/
	outportb(0x43, 0x36);		/* 0x43 is the PIT command channel	*/
	outportb(0x40, divisor & 0xFF);	/* low and high byte of divisor */
	outportb(0x40, divisor >> 8);
}

/*
 * Our IRQ handler
 */
void timer_handler (struct regs *r) {
	putchar ('.');
	timer_ticks++;
	if (timer_ticks % _clockrate == 0) {
		puts ("A second has passed\n");
	}
}

/*
 * Set up clock rate and install irq_handler for timer
 */
void timer_install(int clockrate) {
	_clockrate = clockrate;
	timer_phase(_clockrate);
	irq_install_handler(0,timer_handler);
}
