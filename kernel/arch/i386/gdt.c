#include <stdint.h>
#include <kernel/gdt.h>
#include <stdio.h>

struct gdt_entry gdt[3];
struct gdt_ptr gd;

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
	gdt[num].base_low = (base & 0xFFFF);		// copy lower 2 bytes 
	gdt[num].base_middle = (base >> 16 ) & 0xFF;	// shift 2 bytes, copy low byte
	gdt[num].base_high = (base >> 24 ) & 0xFF;	// shift 3 bytes , copy low byte

	gdt[num].limit_low = (limit & 0xFFFF );		// copy lower 2 bytes
	gdt[num].granularity = ((limit >> 16) & 0x0F);	// shift 2 bytes, copy low nibble
							// high nibble is disregarded
							// high byte disregarded

	gdt[num].granularity |= (gran & 0xF0);		// copy high nibble, disregard low nibble
	gdt[num].access = access;			// copy all
}

void dumpgdt(int i) {
	printf("\nDumping GDT entry [%d]:\n",i);
	struct gdt_entry *e = &gdt[i];
	void *p = e;
	for (int j = 0; j < 8; j++) {
		printf("0x%x ",*(uint8_t *)p);
		p++;
	}
	printf("\n");
}

void gdt_install() {
	gd.limit = (sizeof(struct gdt_entry) * 3) - 1;
	gd.base =(uint32_t) &gdt;

	/*
	 * NULL descriptor
	 */
	gdt_set_gate(0, 0, 0, 0, 0);
	dumpgdt(0);
	/*
	 * Code Segment
	 */
	gdt_set_gate(1, 0x0, 0x000FFFFF, 0x9A, 0xC0);
	dumpgdt(1);
	/*
	 * Data Segment
	 */
	gdt_set_gate(2, 0x0, 0x000FFFFF, 0x92, 0xC0);
	dumpgdt(2);

	//loadgdt(gd.base,gd.limit);
	asm ( "lgdtl %0" : : "m" (gd) );
	refreshGDT();
}
