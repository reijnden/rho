#include <stdint.h>
#include <kernel/gdt.h>
#include <stdio.h>

#define IDT_ENTRIES	256

static struct idt_entry idt[IDT_ENTRIES];

static void idt_set_gate(int num, uint32_t base, uint16_t selector, uint8_t type) {
	idt[num].base_low = (base & 0xFFFF);		// copy lower 2 bytes 
	idt[num].base_high = (base >> 16 ) & 0xFFFF;	// shift 2 bytes , copy lower 2 byte

	idt[num].selector = selector;
	idt[num].type = type;	// 0x8e

	idt[num].zero = 0x00;
}

void idt_install() {
	static struct idt_ptr id;
	id.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
	id.base =(uint32_t) &idt;

	for (int i = 0; i < IDT_ENTRIES; i++) {
		idt_set_gate(i,(uint32_t)&isr_wrapper,0x0008,0x8e);
	}

	asm ( "lidtl %0" : : "m" (id) );
}

void interrupt_handler(){
	printf ("Interrupt!\n");
}
