#include <stdint.h>
#include <stdio.h>
#include <kernel/regs.h>
#include <kernel/flags.h>
#include <kernel/irq.h>
#include <kernel/idt.h>
#include <kernel/ioport.h>

#define PIC_MASTER		0x20
#define PIC_SLAVE		0xA0
#define END_OF_INTERRUPT	0x20

/*
 * An array of IRQ Handler functions
 */
static void *irq_h[16] = { 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0
};

unsigned int irq_on() {
	return (eflags()>>9 & 0x1);
}

/*
 * Enable Interrupts
 */
void irq_enable() {
	__asm__ __volatile__ ( "sti" );
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
 * Program the PIC to remap irq's 0 - 15 to channel 32 - 47
 */
static void remap_irq() {
	/* outportb(port,data) */
	outportb(PIC_MASTER,0x11);
	outportb(PIC_SLAVE,0x11);
	/* remap to 0x20 (32) */
	outportb(PIC_MASTER+1,0x20);
	/* remap to 0x28 (40) */
	outportb(PIC_SLAVE+1,0x28);

	outportb(PIC_MASTER+1,0x04);
	outportb(PIC_SLAVE+1,0x02);

	outportb(PIC_MASTER+1,0x01);
	outportb(PIC_SLAVE+1,0x01);
	/* enable irqs */
	outportb(PIC_MASTER+1,0x0);
	outportb(PIC_SLAVE+1,0x0);
}

void irq_handler(struct regs *r) {
	void (*handler)(struct regs *r);

	handler = irq_h[r->int_no-32];
	if (handler) {
		handler(r);
	}
	/*
	 * Send EOI to slave PIC
	 */
	if (r->int_no >= 40) {
		outportb(PIC_SLAVE,END_OF_INTERRUPT);
	}
	/*
	 * Send EOI to master PIC
	 */
	outportb(PIC_MASTER,END_OF_INTERRUPT);
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
