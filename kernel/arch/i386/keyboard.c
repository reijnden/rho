
#include <kernel/iobus.h>
#include <kernel/irq.h>
#include <kernel/regs.h>
#include <stdio.h>
#include <stdbool.h>

/*
 * The keyboard channels
 */
#define KB_DATAPORT	0x60
#define KB_STATUSPORT	0x64

/* Scancodes for control characters */
#define LSHFT		0x2a
#define RSHFT		0x2a
#define BCKSPC		0x1d
/* flags */
#define UC		0x80
#define BS		0x40
/* Release bit */
#define KEY_UP		0x80

/*
 * space for storing control keys
 */
unsigned char kbflags = 0x0;
/*
 * A very simple keyboard layout
 */
unsigned char kbmap[] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8',		/* 10 */
       	'9', '0', '-', '=', 0,'\t','q','w','e',	'r',		/* 20 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '\b',	/* 30 */
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
	/* uppercase table starts here ( |=UC )*/
	0, 0, '!', '@', '#', '$', '%', '^', '&', '*',		/* 10 */
       	'(', ')', '_', '+', 0, '\t', 'Q', 'W', 'E', 'R',	/* 20 */
       	'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '\b',	/* 30 */
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
void keyboard_handler (struct regs *r) {
	uint8_t status = inportb(KB_STATUSPORT);
	/*
	if (status ^ 0x01) {
		return;
	}
	*/
	uint8_t scancode = inportb(KB_DATAPORT);
	if (scancode & KEY_UP) {				/* release */
		scancode &= ~KEY_UP;				/* switch off bit 7 to get the original key */
		if ((scancode == RSHFT) || (scancode == LSHFT))
			kbflags &= ~UC;				/* uppercase flag off */
		if (scancode == BCKSPC) {
			kbflags &= ~BS;				/* backspace flag off */
			putchar (kbmap[scancode + kbflags]);	/* process backspace on release */
		}
		return;
	}
	if (kbflags & BS)					/* we process nothing until backspace release */
		return;
	if ((scancode == RSHFT) || (scancode == LSHFT))
		kbflags |= UC;					/* uppercase flag on */
	else if (scancode == BCKSPC)
		kbflags |= BS;					/* backspace flag on */
	else if (kbmap[scancode])
		putchar (kbmap[scancode + kbflags]);
}

/*
 * Install irq_handler for keyboard
 */
void keyboard_install() {
	irq_install_handler(1,keyboard_handler);
}
