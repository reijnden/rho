#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#include <stdint.h>

struct gdt_entry {
	uint16_t limit_low;	// bits 0-15 limit
	uint16_t base_low;	// bits 16-31 base
	uint8_t base_middle;	// bits 32-39 base
	uint8_t access;		// bits 40-47 access
	uint8_t granularity;	// bits 48-51 limit and 52-55 flags
	uint8_t base_high;	// bits 56-63 base
}__attribute__((packed));


struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

struct idt_entry {
	uint16_t base_low;	// bits 0-15 base
	uint16_t selector;	// segment selector in GDT/LDT
	uint8_t zero;		// unused: 0x00
	uint8_t type;		// type and attributes
	uint16_t base_high;	// bits 16-31 base
}__attribute__((packed));

struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

void gdt_install();
void idt_install();

/*
 * Defined in loadgdt.S
 */
extern void refreshGDT();
/*
 * Defined in irs.S
 */
extern void isr_wrapper();

#endif
