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

struct regs {
	unsigned int gs,fs,es,ds;
	unsigned int edi,esi,ebp,esp,ebx,edx,ecx,eax;
	unsigned int int_no, err_code;
	unsigned int eip,cs,eflags,useresp,ss;
};

void gdt_install();
void idt_install();

/*
 * Defined in refreshgdt.S
 */
extern void refresh_gdt();
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
