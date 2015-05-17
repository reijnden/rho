#include <stdint.h>
#include <kernel/gdt.h>
#include <kernel/regs.h>
#include <stdio.h>

#define GDT_ENTRIES	3
/*
 * Bitmask for access bits in GDT entry
 * access is 8 bits
 * bit 7 (P)	the present bit, set to 1 for all entries
 * bit 56 (DPL)	define privilege level ring 0-3. 0=kernel, 3=user
 * bit 4 (?)	1
 * bit 3 (EX)	is the executable bit. 1 = code, 0 = data
 * bit 2 (DC)	direction bit for data. 0=segment grows up. 1=down
 *		confirming bit for code.0=given ring only. 1=ring or lower
 * bit 1 (RW)	the rw bit. 1=read access for code/write access for data.
 * bit 0 (AC)	the accessed bit. set to 0. cpu will manage.
 */
#define AC_P	0x80
#define AC_DPL0	0x0				/* DPL: ring levels */
#define AC_DPL1	0x20
#define AC_DPL2	0x40
#define AC_DPL3	0x60
#define AC_EX	0x8				/* EX bit */
#define AC_DC	0x4				/* DC bit */
#define AC_RW	0x2				/* RW bit */
#define AC_AC	0x1				/* AC bit */
#define AC	AC_P | 0x10			/* bare entry: present bit and bit 4 set */

static struct gdt_entry gdt[GDT_ENTRIES];

/*
 *	Input 32+32+8+8=80bit
 *	GDT entry is 64bit
 *
 * base:	32 bit value 
 * limit:	20 bit value	12 bits are ignored
 * access:	8 bit
 * gran:	4 bit		 4 ignored
 *		64		16
 */
static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdt[num].base_low = (base & 0xFFFF);		/* copy lower 2 bytes */
	gdt[num].base_middle = (base >> 16 ) & 0xFF;	/* shift 2 bytes, copy low byte */
	gdt[num].base_high = (base >> 24 ) & 0xFF;	/* shift 3 bytes , copy low byte */
	gdt[num].limit_low = (limit & 0xFFFF );		/* copy lower 2 bytes */
	gdt[num].granularity = ((limit >> 16) & 0x0F);	/* shift 2 bytes, copy low nibble */
							/* high nibble is disregarded */
							/* high byte disregarded */
	gdt[num].granularity |= (gran & 0xF0);		/* copy high nibble, disregard low nibble */
	gdt[num].access = access;			/* copy all */
}

void gdt_install() {
	static struct gdt_ptr gd;
	gd.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
	gd.base =(uint32_t) &gdt;
	gdt_set_gate(0, 0, 0, 0, 0);			 /* NULL descriptor */
	gdt_set_gate(1, 0x0, 0x000FFFFF, 
			AC|AC_DPL0|AC_EX|AC_RW, 0xC0);	 /* Code Segment (0x9a) */
	gdt_set_gate(2, 0x0, 0x000FFFFF, 
			AC|AC_DPL0|AC_RW, 0xC0);	 /* Data Segment (0x92) */
	asm ( "lgdtl %0" : : "m" (gd) );		 /* Load table into register */
	refresh_gdt();					 /* Do the magic */
}

