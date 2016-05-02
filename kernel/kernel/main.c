/*
 * When the boot loader invokes the 32-bit operating system, the machine must have the following state:
 *
 * ‘EAX’
 *	 Must contain the magic value ‘0x2BADB002’; the presence of this value indicates to the operating
 *	 system that it was loaded by a Multiboot-compliant boot loader (e.g. as opposed to another type
 *	 of boot loader that the operating system can also be loaded from).
 * ‘EBX’
 *	Must contain the 32-bit physical address of the Multiboot information structure provided by the
 *	 boot loader.
 * ‘CS’
 *	 Must be a 32-bit read/execute code segment with an offset of ‘0’ and a limit of ‘0xFFFFFFFF’.
 *	 The exact value is undefined.
 * ‘DS’
 * ‘ES’
 * ‘FS’
 * ‘GS’
 * ‘SS’
 *	Must be a 32-bit read/write data segment with an offset of ‘0’ and a limit of ‘0xFFFFFFFF’. The
 *	 exact values are all undefined.
 * ‘A20 gate’
 *	Must be enabled.
 * ‘CR0’
 *	Bit 31 (PG) must be cleared. Bit 0 (PE) must be set. Other bits are all undefined.
 * ‘EFLAGS’
 *	Bit 17 (VM) must be cleared. Bit 9 (IF) must be cleared. Other bits are all undefined.
 *
 * All other processor registers and flag bits are undefined. This includes, in particular:
 *
 * ‘ESP’
 *	The OS image must create its own stack as soon as it needs one.
 * ‘GDTR’
 *	Even though the segment registers are set up as described above, the ‘GDTR’ may be invalid, so
 *	the OS image must not load any segment registers (even just reloading the same values!) until it
 *	sets up its own ‘GDT’.
 * ‘IDTR’
 *	The OS image must leave interrupts disabled until it sets up its own IDT.
 *
 * However, other machine state should be left by the boot loader in normal working order, i.e. as
 * initialized by the bios (or DOS, if that's what the boot loader runs from). In other words, the
 * operating system should be able to make bios calls and such after being loaded, as long as it does
 * not overwrite the bios data structures before doing so. Also, the boot loader must leave the pic
 * programmed with the normal bios/DOS values, even if it changed them during the switch to 32-bit mode.
 */
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
	/*
	 * Storing some data out of the BDA (BIOS Data Area)
	 * See www.bioscentral.com/misc/dba.htm
	 */
	memcpy(&rho.iobase,(uint16_t *)0x463,sizeof(uint16_t));
	memcpy(&rho.displaymode,(uint8_t *)0x449,sizeof(uint8_t));
	memcpy(&rho.cols,(uint8_t *)0x44A,sizeof(uint8_t));
	memcpy(&rho.rows,(uint8_t *)0x484,sizeof(uint8_t));
	terminal_initialize();
	puts("Checking multiboot compliance");
	if (!(magic & MULTIBOOT_MAGIC)) {
		puts ("Not multiboot compliant! Aborting");
		abort();
	}
	printf ("BDA:Video base IO port: 0x%x\n",rho.iobase);
	printf ("BDA:Display mode: 0x%x\n",rho.displaymode);
	printf ("BDA:Number of columns in text mode: %d\n",(unsigned int)rho.cols);
	printf ("BDA:Number of rows in text mode: %d\n",((unsigned int)rho.rows) + 1);
}

/*
 * Called from boot.S, after global constructor
 */
void kernel_main(multiboot_info *mbt)
{
	/*
	 * second argument is an 8 bit flag. low bit sets verbosity.
	 */
	boot_info(mbt,MB_DRIVE | MB_DEV);
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
