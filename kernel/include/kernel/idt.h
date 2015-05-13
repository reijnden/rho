#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h>

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

void idt_install();
void idt_set_gate(int num, uint32_t base, uint16_t selector, uint8_t type);

/*
 * Defined in irs.S
 */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

#endif
