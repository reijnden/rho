#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <kernel/core.h>
#include <kernel/tty.h>
#include <kernel/multiboot.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>

/*
 * Called from boot.S, prior to global constructor
 */
void kernel_early(uint32_t magic)
{
	irq_disable();
	terminal_initialize();
	printf("Checking multiboot compliance [0x%lx & 0x%x]...\n",magic,MULTIBOOT_MAGIC);
	if (!(magic & MULTIBOOT_MAGIC)) {
		printf ("Not multiboot compliant! %s\n","Aborting");
		abort();
	}
}

/*
 * Called from boot.S, after global constructor
 */
void kernel_main(multiboot_info *mbt)
{
	boot_info(mbt,(uint8_t)0x00);
	printf ("Setting up Global Descriptor Table... ");
	gdt_install();
	printf ("OK\n");
	printf ("Setting up Interrupt Descriptor Table... ");
	idt_install();
	printf ("OK\n");
	printf ("Setting up IRQ... ");
	irq_install();
	printf ("OK\n");
	printf ("Configuring system clock... ");
	timer_install(100);
	printf ("OK\n");
	printf ("Enabling keyboard... ");
	keyboard_install();
	printf ("OK\n");
	printf ("Enabling interrupts... ");
	irq_enable();
	if (irq_on()) {
		printf ("OK\n");
	} else {
		abort();
	}
	/*
	 * Testing printf
	for ( unsigned int i = 0; i<16;i++) {
		printf("Int:%i (%u bits) Hex:0x%x Oct:0%o\n",i,(unsigned int)sizeof(i) * 8,i,i);
	}
	for ( unsigned long l = 0; l<16;l++) {
		printf("Long:%lu (%u bits)\n",l,(unsigned int)sizeof(l) * 8);
	}
	for ( unsigned long long l = 0; l<16;l++) {
		printf("Longlong:%llu (%u bits)\n",l,(unsigned int)sizeof(l) * 8);
	}
	 */
	/*
	 * Testing cr0 register
	 */
	uint32_t _cr0 = cr0();
	printf("PE (Protection Enabled, lowest bit of cr0 register) set? 0x%lx\n",_cr0);

	printf("Rho version %d.%d.%d booted\n",RHO_MAJOR,RHO_MINOR,RHO_PATCH);
	/*
	 * Wait until interrupted
	 */
	while ( 1 ) {
		/*
		 * Update the cursos position
		 */
		upd_c();
		/*
		 * Wait for interrupts
		 */
		irq_wait();
	}
	/* */
}
