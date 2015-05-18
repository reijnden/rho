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
#define AC_P		0x80
#define AC_DPL0		0x0				/* DPL: ring levels */
#define AC_DPL1		0x20
#define AC_DPL2		0x40
#define AC_DPL3		0x60
#define AC_EX		0x8				/* EX bit */
#define AC_DC		0x4				/* DC bit */
#define AC_RW		0x2				/* RW bit */
#define AC_AC		0x1				/* AC bit */
#define AC		AC_P | 0x10			/* bare entry: present bit and bit 4 set */
#define F_GR		0x8				/* Granularity bit */
#define F_SZ		0x4				/* Size bit for 32bit pm */

#define GDT_FLGS	((F_GR | F_SZ) << 20)
#define KERNEL_CODE	AC | AC_DPL0 | AC_EX | AC_RW
#define KERNEL_DATA	AC | AC_DPL0 | AC_RW

static struct gdt_entry gdt[GDT_ENTRIES];

/*
 *	Input 32+32+8=72bit
 *	GDT entry is 64bit
 *
 * base:	32 bit value 
 * limit:	32 bit value	lower 16 are lim 0:15
 *				next 8 are flg|lim
 *				next 8 are ignored
 * access:	8 bit
 *
 *	Layout GDT Entry:
 *	bs = base
 *	lim = limit
 *	flg = flags
 *
 *	|---- ---- ---- ----|---- ---- ---- ----|
 *	|	bs 0:15     |	lim 0:15	|
 *	|---- ----|----|----|---- ----|---- ----|
 *	|bs 24:31 |flg |lim | access  | bs 16:23|
 *	|---- ----|----|----|---- ----|---- ----|
 *	Flags:	Gr,Sz,'0','0'
 *		Gr: granularity 0=byte, 1=4Kb blocks
 *		Sz: 0=16bit protected mode,1=32bit pm
 *	Access:	See definition above
 *
 *		Let's try this:
 *		0x100000 (1M) Kernel code (0x100 in 4K-blocks (0x1000))
 *		0x100000 (1M) Kernel data
 *		0x100000 (1M) IRQ/ISR
 *		0x100000 (1M) User  code
 *		0x100000 (1M) User data 
 */
static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access) {
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16 ) & 0xFF;
	gdt[num].base_high = (base >> 24 ) & 0xFF;
	gdt[num].limit_low = (limit & 0xFFFF );	
	gdt[num].granularity = ((limit >> 16) & 0xFF);
	gdt[num].access = access;
}

void gdt_install() {
	static struct gdt_ptr gd;
	gd.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
	gd.base =(uint32_t) &gdt;
	gdt_set_gate(0, 0, 0, 0);			/* NULL descriptor */
	gdt_set_gate(1, 0x0,				/* Kernel code */
		0x000FFFFF | GDT_FLGS,
		KERNEL_CODE);
	gdt_set_gate(2, 0x0,				/* Kernel data */
		0x000FFFFF | GDT_FLGS,
		KERNEL_DATA);
	asm ( "lgdtl %0" : : "m" (gd) );		/* Load table into register */
	refresh_gdt();					/* Do the magic */
}

