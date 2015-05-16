
#include <kernel/iobus.h>
#include <kernel/irq.h>
#include <kernel/regs.h>
#include <stdio.h>
#include <stdbool.h>

#define LSHFT	0x2a
#define RSHFT	0x2a
#define FLAG_UPPER 0x80

unsigned char kb[] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8',		/* 10 */
       	'9', '0', '-', '=', 0,'\t','q','w','e',	'r',		/* 20 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,	/* 30 */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 40 */
	'\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',	/* 50 */
	'm', ',', '.', '/', 0, 0, 0, ' ', 0, 0,			/* 60 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,				/* 70 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,				/* 80 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,				/* 90 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,				/* 100 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,				/* 110 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,				/* 120 */
	0, 0, 0, 0, 0, 0, 0, 0,					/* 128 */
	/* uppercase table starts here ( just add 128 )*/
	0, 0, '!', '@', '#', '$', '%', '^', '&', '*',		/* 10 */
       	'(', ')', '_', '+', 0, '\t', 'Q', 'W', 'E', 'R',	/* 20 */
       	'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,	/* 30 */
       	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 40 */
       	'"', '~', 0, '|', 'U', 'X', 'C', 'V', 'B', 'N',		/* 50 */
       	'M', '<', '>', '?', 0, 0, 0, ' ', 0, 0,			/* 60 */
       	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,				/* 70 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,				/* 80 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,				/* 90 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,				/* 100 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,				/* 110 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,				/* 120 */
	0, 0, 0, 0, 0, 0, 0, 0					/* 128 */
};


/*
 * Our IRQ handler for the keyboard
 * This function presumes that there can be only one keyboard.
 */
unsigned char keyflags = 0x0;
void keyboard_handler (struct regs *r) {
	unsigned char key = inportb(0x60);
	if (key & 0x80)  /* release */
		return;
	if ((key == RSHFT) || (key == LSHFT)) {
		keyflags |= FLAG_UPPER;
	} else if (kb[key]) {
		putchar (kb[key + keyflags]);
		keyflags = 0x0;
	}
}

/*
 * Install irq_handler for keyboard
 */
void keyboard_install() {
	irq_install_handler(1,keyboard_handler);
}
