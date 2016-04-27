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
#include <kernel/kernel.h>

rho_context rho;
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
	memset(&rho,0,sizeof(rho_context));
	rho.cols=80;	// We need some preliminary values in order to write
	rho.rows=25;	// to the console
	terminal_initialize();
	puts("Checking multiboot compliance");
	if (!(magic & MULTIBOOT_MAGIC)) {
		puts ("Not multiboot compliant! Aborting");
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
	boot_info(mbt,MB_MEMORY | MB_BDA);
	printf ("BDA:Video base IO port: 0x%x\n",rho.iobase);
	printf ("BDA:Display mode: 0x%x\n",rho.displaymode);
	printf ("BDA:Number of columns in text mode: %d\n",(unsigned int)rho.cols);
	printf ("BDA:Number of rows in text mode: %d\n",((unsigned int)rho.rows) + 1);

	printf ("Setting up Global Descriptor Table... ");
	gdt_install();
	puts ("OK");
	printf ("Setting up Interrupt Descriptor Table... ");
	idt_install();
	puts ("OK");
	printf ("Setting up IRQ... ");
	irq_install();
	puts ("OK");
	printf ("Configuring system clock... ");
	timer_install(100);
	puts ("OK");
	printf ("Enabling keyboard... ");
	keyboard_install();
	puts ("OK");
	printf ("Enabling interrupts... ");
	irq_enable();
	if (irq_on()) {
		puts ("OK");
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
//	mem_inspect(1024,1536);
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
