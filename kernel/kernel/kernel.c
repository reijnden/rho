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
 * Dump a memory area on screen
 */
void mem_inspect(uint32_t byte_from, uint32_t byte_to){
	/*
	 * Fool the compiler, initiate p on address 1, null-pointers don't work
	 */
	uint8_t *pos = (uint8_t *)0x01;
	pos+=byte_from;
	/*
	 * number of bytes per line
	 */
	int bytes_per_line = 16;
	while (byte_from<byte_to){
		if (byte_from % bytes_per_line == 0)printf("0x%lx:",byte_from);
		printf ("0x%X ",*(pos-1));
		byte_from++;
		pos++;
		if (byte_from % bytes_per_line == 0)printf("\n");
	}
}
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
	/*
	 * second argument is an 8 bit flag. low bit sets verbosity.
	 */
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
	 * Testing cr0 register
	 */
	uint32_t _cr0 = cr0();
	printf("PE (Protection Enabled, lowest bit of cr0 register) set? 0x%lx, %s\n",
			_cr0,_cr0&0x00000001?"yes":"no");

	printf("Rho version %d.%d.%d booted\n",RHO_MAJOR,RHO_MINOR,RHO_PATCH);
	/*
	 * Wait until interrupted
	 */
	mem_inspect(1024,1536);
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
