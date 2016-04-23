#ifndef _KERNEL_IRH_H
#define _KERNEL_IRH_H

#include <kernel/regs.h>
#include <stdbool.h>

void irq_install();
void irq_install_handler (int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_enable();
void irq_disable();
bool irq_on();
void irq_wait();

/*
 * Defined in irq.S
 */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif
