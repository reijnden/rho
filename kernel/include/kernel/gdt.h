#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#include <stdint.h>

struct idt_entry {
	uint16_t offset_1;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_2;
}__attribute__((packed));

struct gdt_entry {
	uint16_t limit_low;	// bits 0-15 limit
	uint16_t base_low;	// bits 16-31 base
	uint8_t base_middle;	// bits 32-39 base
	uint8_t access;	// bits 40-47 access
	uint8_t granularity;	// bits 48-51 limit and 52-55 flags
	uint8_t base_high;	// bits 56-63 base
}__attribute__((packed));

struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

void gdt_install();

/*
 * Defined in loadgdt.S
 */
extern void loadgdt(uint16_t l, uint32_t b);
/*
 * Defined in loadidt.S
 */
extern void loadidt(uint16_t l, uint32_t b);

#endif
