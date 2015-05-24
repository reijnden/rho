#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <kernel/core.h>
#include <kernel/tty.h>

#define BOOTINFO	((multiboot_info *)(h_sys->mbi))

/*
 * Inspect the info provided by the bootloader.
 * Currently it is printed to screen and attached to the system struct
 */
void bootstrap(multiboot_info *mbt, system *h_sys) {
	h_sys->mbi = mbt;
	printf ("Flags [%x]\n",(unsigned int)BOOTINFO->flags);
	if (BOOTINFO->flags & MULTIBOOT_INFO_MEMORY) {
		printf ("Lower memory at 0x0: %u Kb,",(unsigned int)BOOTINFO->mem_lower);
		printf ("Upper memory at 0x100000: %u Kb\n",(unsigned int)BOOTINFO->mem_upper);
	}
	if (BOOTINFO->flags & MULTIBOOT_INFO_BOOTDEV)
		printf ("Boot device: [0x%x]\n", (unsigned int)(BOOTINFO->boot_device));
	if (BOOTINFO->flags & MULTIBOOT_INFO_CMDLINE)
		printf ("Command line: [%s]\n", (char *)(BOOTINFO->cmdline));
	if (BOOTINFO->flags & MULTIBOOT_INFO_MODS)
		printf ("Modules count: [0x%x]\n", (unsigned int)(BOOTINFO->mods_count));
	if (BOOTINFO->flags & MULTIBOOT_INFO_MEM_MAP) {
		printf ("Memory map address: 0x%lx, length: %lu bytes\n", 
				(unsigned long)(BOOTINFO->mmap_addr),
				(unsigned long)(BOOTINFO->mmap_length));
		multiboot_mmap *mmap = (multiboot_mmap *)(BOOTINFO->mmap_addr);
		uint64_t mema = 0;
		uint64_t memr = 0;
		while (mmap < (multiboot_mmap *)(BOOTINFO->mmap_addr + BOOTINFO->mmap_length)) {
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
			mmap = (multiboot_mmap *)((unsigned int)mmap + (unsigned int)(mmap->size) + sizeof(unsigned int));
		}
		printf("Total memory : %lluKb, Reserved: %lluKb, Available: %lluKb\n",
				(mema+memr)/1024, memr/1024, mema/1024);
	}
	if (BOOTINFO->flags & MULTIBOOT_INFO_DRIVE_INFO)
		printf ("Drive info: [0x%x]\n", (unsigned int)(BOOTINFO->drives_length));
	if (BOOTINFO->flags & MULTIBOOT_INFO_CONFIG_TABLE)
		printf ("Config table: [0x%x]\n", (unsigned int)(BOOTINFO->config_table));
	if (BOOTINFO->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME)
		printf ("Bootloader: [%s]\n", (const char *)(BOOTINFO->boot_loader_name));
	if (BOOTINFO->flags & MULTIBOOT_INFO_APM_TABLE)
		printf ("APM table: [0x%x]\n", (unsigned int)(BOOTINFO->apm_table));
	if (BOOTINFO->flags & MULTIBOOT_INFO_VBE_INFO)
		printf ("Graphics table %s\n","available");
}

