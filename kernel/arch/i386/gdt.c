#include <stdint.h>
#include <kernel/gdt.h>

struct gdt_entry gdt[3];

static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16 ) & 0xFF;
	gdt[num].base_high = (base >> 24 ) & 0xFF;

	gdt[num].limit_low = (limit & 0xFFFF );
	gdt[num].granularity = ((limit >> 16) & 0xFF);

	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;
}

void gdt_install() {
	struct gdt_ptr gd;
	gd.limit = (sizeof(struct gdt_entry) * 3) - 1;
	gd.base =(uint32_t) &gdt;

	/*
	 * NULL descriptor
	 */
	gdt_set_gate(0, 0, 0, 0, 0);
	/*
	 * Code Segment
	 */
	gdt_set_gate(1, 0, 0xFFFF, 0x9A, 0xCF);
	/*
	 * Data Segment
	 */
	gdt_set_gate(2, 0, 0xFFFF, 0x92, 0xCF);

	loadgdt(gd.base,gd.limit);
}
