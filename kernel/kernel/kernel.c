#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <kernel/tty.h>
#include <kernel/boot/multiboot.h>

void kernel_early(void)
{
	terminal_initialize();
}


static void boot_info(multiboot_info *mbt)
{
	printf ("Flags [%x]\n",(unsigned int)mbt->flags);
	if (mbt->flags & MULTIBOOT_INFO_MEMORY) {
		printf ("Lower memory at 0x0: %u Kb\n",(unsigned int)mbt->mem_lower);
		printf ("Upper memory at 0x100000: %u Kb\n",(unsigned int)mbt->mem_upper);
	}
	if (mbt->flags & MULTIBOOT_INFO_BOOTDEV) {
		
		printf ("Boot device: [0x%x]\n", (unsigned int)(mbt->boot_device));
	}
	if (mbt->flags & MULTIBOOT_INFO_CMDLINE) {
		
		printf ("Command line: [%s]\n", (char *)(mbt->cmdline));
	}
	if (mbt->flags & MULTIBOOT_INFO_MODS) {
		
		printf ("Modules count: [0x%x]\n", (unsigned int)(mbt->mods_count));
	}
	if (mbt->flags & MULTIBOOT_INFO_MEM_MAP) {
		printf ("Memory map address: 0x%lx, length: %lu bytes\n", 
				(unsigned long)(mbt->mmap_addr),
				(unsigned long)(mbt->mmap_length));
		multiboot_mmap *mmap = (multiboot_mmap *)(mbt->mmap_addr);
		uint64_t mema = 0;
		uint64_t memr = 0;
		while (mmap < mbt->mmap_addr + mbt->mmap_length) {
			if (mmap->type & MULTIBOOT_MEMORY_AVAILABLE)
				mema+=mmap->len;
			if (mmap->type & MULTIBOOT_MEMORY_RESERVED)
				memr+=mmap->len;
			printf ("\tsize: %lu bytes, address: 0x%llx, length: 0x%llx (%llu bytes), type: 0x%lx \n",
				       	mmap->size,
					mmap->addr,
					mmap->len,
					mmap->len,
					mmap->type);
			mmap = (unsigned int)mmap + (unsigned int)(mmap->size) + sizeof(unsigned int);
		}
		printf("Total memory : %lluKb, Reserved: %lluKb, Available: %lluKb\n",
				(mema+memr)/1024, memr/1024, mema/1024);
	}
	if (mbt->flags & MULTIBOOT_INFO_DRIVE_INFO) {
		
		printf ("Drive info: [0x%x]\n", (unsigned int)(mbt->drives_length));
	}
	if (mbt->flags & MULTIBOOT_INFO_CONFIG_TABLE) {
		
		printf ("Config table: [0x%x]\n", (unsigned int)(mbt->config_table));
	}
	if (mbt->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME) {
		printf ("Bootloader: [%s]\n", (const char *)(mbt->boot_loader_name));
	}
	if (mbt->flags & MULTIBOOT_INFO_APM_TABLE) {
		
		printf ("APM table: [0x%x]\n", (unsigned int)(mbt->apm_table));
	}
	if (mbt->flags & MULTIBOOT_INFO_VBE_INFO) {
		
		printf ("Graphics table %s\n","available");
	}
}

void kernel_main(int magic, multiboot_info *mbt)
{
	printf("Checking multiboot compliance [0x%x & 0x%x]...\n",magic,MULTIBOOT_MAGIC);
	if (!(magic & MULTIBOOT_MAGIC)) {
		printf ("Not multiboot compliant! %s\n","Aborting");
		abort();
	}
	boot_info(mbt);
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
	 * Tesing asm
	unsigned int cr0;
	asm ("movl %%cr0,%%eax\n"
	     "movl %%eax, %0" : "=rm" (cr0) );
	printf("PE set by bootloader? cr0 register: 0x%x\n",cr0);
	 */
}
