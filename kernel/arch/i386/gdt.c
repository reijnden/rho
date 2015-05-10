#include <stdint.h>
#include <kernel/gdt.h>

#define GDT_ENTRIES	3
#define IDT_ENTRIES	256

struct gdt_entry gdt[GDT_ENTRIES];
struct idt_entry idt[IDT_ENTRIES];

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
	gd.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
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

void idt_install() {
	struct gdt_ptr gd;
	gd.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
	gd.base =(uint32_t) &idt;
	loadidt(gd.base,gd.limit);
}
