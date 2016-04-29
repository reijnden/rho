#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <kernel/tty.h>
#include <kernel/multiboot.h>
#include <kernel/kernel.h>

/*
 * Inspect the info provided by the bootloader.
 * Currently it is printed to screen.
 * For example the memory map and commandline can be usefull ;-)
 * flags: see multiboot.h
 *
 */
void boot_info(multiboot_info *mbt,uint16_t flags)
{
	char *mtype[5] = {"Available","Reserved","ACPI reclaimable","NVS","Bad RAM"};
	if (mbt->flags & MULTIBOOT_INFO_MEMORY) {
		if (flags & MB_MEMORY)
			printf ("Lower memory at 0x0: %u %s\n",
				(unsigned int)mbt->mem_lower>>10>0?
					(unsigned int)mbt->mem_lower>>10:
					(unsigned int)mbt->mem_lower,
				(unsigned int)mbt->mem_lower>>10>0?"Mb":"Kb");
		if (flags & MB_MEMORY)
			printf ("Upper memory at 0x100000: %u %s\n",
					(unsigned int)mbt->mem_upper>>10>0?
						(unsigned int)mbt->mem_upper>>10:
						(unsigned int)mbt->mem_upper,
					(unsigned int)mbt->mem_upper?"Mb":"Kb");
	}
	if (mbt->flags & MULTIBOOT_INFO_BOOTDEV) {
		if (flags & MB_DEV)
			printf ("Boot device: [0x%x]\n", (unsigned int)(mbt->boot_device));
	}
	if (mbt->flags & MULTIBOOT_INFO_CMDLINE) {
		if (flags & MB_CMD)
			printf ("Command line: [%s]\n", (char *)(mbt->cmdline));
	}
	if (mbt->flags & MULTIBOOT_INFO_MODS) {
		if (flags & MB_MODS)
			printf ("Modules count: [0x%x]\n", (unsigned int)(mbt->mods_count));
	}
	if (mbt->flags & MULTIBOOT_INFO_MEM_MAP) {
		if (flags & MB_MEMORY) printf ("Memory map address: 0x%lx, length: %lu bytes\n",
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
			if (flags & MB_MEMORY)
				printf ("sz:%lu bytes, addr:0x%llx, len:0x%llx (%llu %s), type:0x%lx (%s)\n",
				       	mmap->size,
					mmap->addr,
					mmap->len,
					mmap->len>>10>0?(mmap->len>>20>0?mmap->len>>20:mmap->len>>10):mmap->len,
					mmap->len>>10>0?(mmap->len>>20>0?"Mb":"Kb"):"B",
					mmap->type,
					mtype[mmap->type - 1]);
			mmap = (multiboot_mmap *)((unsigned int)mmap + (unsigned int)(mmap->size) + sizeof(unsigned int));
		}
		if (flags & MB_MEMORY)
			printf("Total memory : %llu %s, Reserved: %llu %s, Available: %llu %s\n",
				/* >>10 == divide by 1024 */
				(mema+memr)>>10>0?((mema+memr)>>20>0?(mema+memr)>>20:(mema+memr)>>10):mema+memr,
				(mema+memr)>>10>0?((mema+memr)>>20>0?"Mb":"Kb"):"bytes",
				memr>>10>0?(memr>>20>0?memr>>20:memr>>10):memr,
				memr>>10>0?(memr>>20>0?"Mb":"Kb"):"bytes",
				mema>>10>0?(mema>>20>0?mema>>20:mema>>10):mema,
				mema>>10>0?(mema>>20>0?"Mb":"Kb"):"bytes"
				);
	}
	if (mbt->flags & MULTIBOOT_INFO_DRIVE_INFO) {
		if (flags & MB_DRIVE)
			printf ("Drive info: [0x%x]\n", (unsigned int)(mbt->drives_length));
	}
	if (mbt->flags & MULTIBOOT_INFO_CONFIG_TABLE) {
		if (flags & MB_CONFIG)
			printf ("Config table: [0x%x]\n", (unsigned int)(mbt->config_table));
	}
	if (mbt->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME) {
		if (flags & MB_DEV)
			printf ("Bootloader: [%s]\n", (const char *)(mbt->boot_loader_name));
	}
	if (mbt->flags & MULTIBOOT_INFO_APM_TABLE) {
		if (flags & MB_APM)
			printf ("APM table: [0x%x]\n", (unsigned int)(mbt->apm_table));
	}
	if (mbt->flags & MULTIBOOT_INFO_VBE_INFO) {
		if (flags & MB_VBE)
			printf ("Graphics table %s\n","available");
	}
}

