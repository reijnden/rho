#include <stdint.h>
#include <stdio.h>
#include <kernel/regs.h>
#include <kernel/irq.h>
#include <kernel/idt.h>
#include <kernel/ioport.h>

/*
 * An array of IRQ Handler functions
 */
static void *irq_h[16] = { 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0
};

/*
 * Enable Interrupts
 */
void irq_enable() {
	__asm__ __volatile__ ( "cli" );
}

/*
 * Disable Interrupts
 */
void irq_disable() {
	__asm__ __volatile__ ( "cli" );
}

/*
 * Install a handler
 */
void irq_install_handler (int irq, void (*handler)(struct regs *r)) {
	irq_h[irq] = handler;
}
/*
 * Uninstall a handler
 */
void irq_uninstall_handler(int irq) {
	irq_h[irq] = 0;
}

/*
 * Program the PIC to remap irq's 0 - 7 to channel 8 - 15
 */
static void remap_irq() {
	outportb(0x20,0x11);
	outportb(0xa0,0x11);
	outportb(0x21,0x20);
	outportb(0xa1,0x28);
	outportb(0x21,0x04);
	outportb(0xa1,0x02);
	outportb(0x21,0x01);
	outportb(0xa1,0x01);
	outportb(0x21,0x0);
	outportb(0xa1,0x0);
}

void irq_handler(struct regs *r) {
	void (*handler)(struct regs *r);
	printf ("IRQ_handler\n");

	handler = irq_h[r->int_no-32];
	if (handler) {
		handler(r);
	}
	/*
	 * Send EOI to slave PIC
	 */
	if (r->int_no >= 40) {
		outportb(0xa0,0x20);
	}
	/*
	 * Send EOI to master PIC
	 */
	outportb(0x20,0x20);
}

void irq_install() {
	remap_irq();
	idt_set_gate(32,(uint32_t)irq0,0x08,0x8e);
	idt_set_gate(33,(uint32_t)irq1,0x08,0x8e);
	idt_set_gate(34,(uint32_t)irq2,0x08,0x8e);
	idt_set_gate(35,(uint32_t)irq3,0x08,0x8e);
	idt_set_gate(36,(uint32_t)irq4,0x08,0x8e);
	idt_set_gate(37,(uint32_t)irq5,0x08,0x8e);
	idt_set_gate(38,(uint32_t)irq6,0x08,0x8e);
	idt_set_gate(39,(uint32_t)irq7,0x08,0x8e);
	idt_set_gate(40,(uint32_t)irq8,0x08,0x8e);
	idt_set_gate(41,(uint32_t)irq9,0x08,0x8e);
	idt_set_gate(42,(uint32_t)irq10,0x08,0x8e);
	idt_set_gate(43,(uint32_t)irq11,0x08,0x8e);
	idt_set_gate(44,(uint32_t)irq12,0x08,0x8e);
	idt_set_gate(45,(uint32_t)irq13,0x08,0x8e);
	idt_set_gate(46,(uint32_t)irq14,0x08,0x8e);
	idt_set_gate(47,(uint32_t)irq15,0x08,0x8e);
}
