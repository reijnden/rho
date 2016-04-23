#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <kernel/tty.h>
#include <kernel/multiboot.h>

/*
 * Inspect the info provided by the bootloader.
 * Currently it is printed to screen.
 * TODO: store the info for later usage.
 * For example the memory map and commandline can be usefull ;-)
 */
void boot_info(multiboot_info *mbt,uint8_t flags)
{
	if (flags&0x01) printf ("Flags [%x]\n",(unsigned int)mbt->flags);
	if (mbt->flags & MULTIBOOT_INFO_MEMORY) {
		if (flags&0x01) printf ("Lower memory at 0x0: %u Kb\n",(unsigned int)mbt->mem_lower);
		if (flags&0x01) printf ("Upper memory at 0x100000: %u Kb\n",(unsigned int)mbt->mem_upper);
	}
	if (mbt->flags & MULTIBOOT_INFO_BOOTDEV) {
		
		if (flags&0x01) printf ("Boot device: [0x%x]\n", (unsigned int)(mbt->boot_device));
	}
	if (mbt->flags & MULTIBOOT_INFO_CMDLINE) {
		
		if (flags&0x01) printf ("Command line: [%s]\n", (char *)(mbt->cmdline));
	}
	if (mbt->flags & MULTIBOOT_INFO_MODS) {
		
		if (flags&0x01) printf ("Modules count: [0x%x]\n", (unsigned int)(mbt->mods_count));
	}
	if (mbt->flags & MULTIBOOT_INFO_MEM_MAP) {
		if (flags&0x01) printf ("Memory map address: 0x%lx, length: %lu bytes\n",
				(unsigned long)(mbt->mmap_addr),
				(unsigned long)(mbt->mmap_length));
		multiboot_mmap *mmap = (multiboot_mmap *)(mbt->mmap_addr);
		uint64_t mema = 0;
		uint64_t memr = 0;
		while (mmap < (multiboot_mmap *)(mbt->mmap_addr + mbt->mmap_length)) {
			if (mmap->type & MULTIBOOT_MEMORY_AVAILABLE)
				mema+=mmap->len;
			if (mmap->type & MULTIBOOT_MEMORY_RESERVED)
				memr+=mmap->len;
			if (flags&0x01) printf ("\tsize: %lu bytes, address: 0x%llx, length: 0x%llx (%llu bytes), type: 0x%lx \n",
				       	mmap->size,
					mmap->addr,
					mmap->len,
					mmap->len,
					mmap->type);
			mmap = (multiboot_mmap *)((unsigned int)mmap + (unsigned int)(mmap->size) + sizeof(unsigned int));
		}
		if (flags&0x01) printf("Total memory : %lluKb, Reserved: %lluKb, Available: %lluKb\n",
				/* >>10 == divide by 1024 */
				(mema+memr)>>10, memr>>10, mema>>10);
	}
	if (mbt->flags & MULTIBOOT_INFO_DRIVE_INFO) {
		
		if (flags&0x01) printf ("Drive info: [0x%x]\n", (unsigned int)(mbt->drives_length));
	}
	if (mbt->flags & MULTIBOOT_INFO_CONFIG_TABLE) {
		
		if (flags&0x01) printf ("Config table: [0x%x]\n", (unsigned int)(mbt->config_table));
	}
	if (mbt->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME) {
		if (flags&0x01) printf ("Bootloader: [%s]\n", (const char *)(mbt->boot_loader_name));
	}
	if (mbt->flags & MULTIBOOT_INFO_APM_TABLE) {
		
		if (flags&0x01) printf ("APM table: [0x%x]\n", (unsigned int)(mbt->apm_table));
	}
	if (mbt->flags & MULTIBOOT_INFO_VBE_INFO) {
		
		if (flags&0x01) printf ("Graphics table %s\n","available");
	}
	/*
	 * Printing some data out of the BDA (BIOS Data Area)
	 * See www.bioscentral.com/misc/dba.htm
	 */
	uint16_t *iobase = (uint16_t *)0x463;
	if (flags&0x01) printf ("Video base IO port: 0x%x\n",*iobase);
	uint8_t *displaymode = (uint8_t *)0x449;
	if (flags&0x01) printf ("Display mode: 0x%x\n",*displaymode);
	uint8_t *cols = (uint8_t *)0x44A;
	if (flags&0x01) printf ("Number of columns in text mode: %d\n",(unsigned int)*cols);
	uint8_t *rows = (uint8_t *)0x484;
	if (flags&0x01) printf ("Number of rows in text mode: %d\n",((unsigned int)*rows) + 1);
}

