
#include <kernel/ioport.h>
#include <kernel/irq.h>
#include <kernel/regs.h>
#include <stdio.h>
#include <stdbool.h>

unsigned char kb[] = {
/*	0	1	2	3	4	5	6	7	8	9	a	*/
	0,	0,	'1',	'2',	'3',	'4',	'5',	'6',	'7',	'8',	'9',
/*	b	c	d	e	f	10	11	12	13	14	15	*/
	'0',	'-',	'=',	0,	'\t',	'q',	'w',	'e',	'r',	't',	'y',
/*	16	17	18	19	1a	1b	1c	1d	1e	1f	20	*/
	'u',	'i',	'o',	'p',	'[',	']',	'\n',	0,	'a',	's',	'd',
/*	21	22	23	24	25	26	27	28	29	2a	2b	*/
	'f',	'g',	'h',	'j',	'k',	'l',	';',	'\'',	'`',	0,	'\\',
/*	2c	2d	2e	2f	30	31	32	33	34	35	36	*/
	'z',	'x',	'c',	'v',	'b',	'n',	'm',	',',	'.',	'/',	0,
/*	37	38	39	3a	3b	3c	3d	3e	3f	40	41	*/
	0,	0,	' ',	0,	0,	0,	0,	0,	0,	0,	0,
/*	42	43	44	45	46	47	48	49	4a	4b	4c	*/
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
/*	4d	4e	4f	50	51	52	53	54	55	56	57	*/
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
/*	58	59	5a	5b	5c	5d	5e	5f	60	61	62	*/
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0
};
unsigned char kbup[] = {
/*	0	1	2	3	4	5	6	7	8	9	a	*/
	0,	0,	'!',	'@',	'#',	'$',	'%',	'^',	'&',	'*',	'(',
/*	b	c	d	e	f	10	11	12	13	14	15	*/
	')',	'_',	'+',	0,	'\t',	'Q',	'W',	'E',	'R',	'T',	'Y',
/*	16	17	18	19	1a	1b	1c	1d	1e	1f	20	*/
	'U',	'I',	'O',	'P',	'{',	'}',	'\n',	0,	'A',	'S',	'D',
/*	21	22	23	24	25	26	27	28	29	2a	2b	*/
	'F',	'G',	'H',	'J',	'K',	'L',	':',	'"',	'~',	0,	'|',
/*	2c	2d	2e	2f	30	31	32	33	34	35	36	*/
	'U',	'X',	'C',	'V',	'B',	'N',	'M',	'<',	'>',	'?',	0,
/*	37	38	39	3a	3b	3c	3d	3e	3f	40	41	*/
	0,	0,	' ',	0,	0,	0,	0,	0,	0,	0,	0,
/*	42	43	44	45	46	47	48	49	4a	4b	4c	*/
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
/*	4d	4e	4f	50	51	52	53	54	55	56	57	*/
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
/*	58	59	5a	5b	5c	5d	5e	5f	60	61	62	*/
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0
};

/*
 * Our IRQ handler for the keyboard
 * This function presumes that there can be only one keyboard.
 */
bool upp = false;
void keyboard_handler (struct regs *r) {

	unsigned char key = inportb(0x60);

	if (key & 0x80) { /* release */
//		printf("0x%x",key);
		return;
	}
	if (key == 0x2a) {
		upp = true;
		return;
	}
	if (kb[key]) {
		putchar (upp?kbup[key]:kb[key]);
		upp = false;
	} else {
		//printf("0x%x",key);
	}
}

/*
 * Install irq_handler for keyboard
 */
void keyboard_install() {
	irq_install_handler(1,keyboard_handler);
}
