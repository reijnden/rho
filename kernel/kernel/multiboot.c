/*
 * The Multiboot information structure and its related substructures may be placed anywhere in memory
 * by the boot loader (with the exception of the memory reserved for the kernel and boot modules, of
 * course). It is the operating system's responsibility to avoid overwriting this memory until it is
 * done using it.
 *
 * The format of the Multiboot information structure (as defined so far) follows:
 *
 *            +-------------------+
 *    0       | flags             |    (required)
 *            +-------------------+
 *    4       | mem_lower         |    (present if flags[0] is set)
 *    8       | mem_upper         |    (present if flags[0] is set)
 *            +-------------------+
 *    12      | boot_device       |    (present if flags[1] is set)
 *            +-------------------+
 *    16      | cmdline           |    (present if flags[2] is set)
 *            +-------------------+
 *    20      | mods_count        |    (present if flags[3] is set)
 *    24      | mods_addr         |    (present if flags[3] is set)
 *            +-------------------+
 *    28 - 40 | syms              |    (present if flags[4] or
 *            |                   |                flags[5] is set)
 *            +-------------------+
 *    44      | mmap_length       |    (present if flags[6] is set)
 *    48      | mmap_addr         |    (present if flags[6] is set)
 *            +-------------------+
 *    52      | drives_length     |    (present if flags[7] is set)
 *    56      | drives_addr       |    (present if flags[7] is set)
 *            +-------------------+
 *    60      | config_table      |    (present if flags[8] is set)
 *            +-------------------+
 *    64      | boot_loader_name  |    (present if flags[9] is set)
 *            +-------------------+
 *    68      | apm_table         |    (present if flags[10] is set)
 *            +-------------------+
 *    72      | vbe_control_info  |    (present if flags[11] is set)
 *    76      | vbe_mode_info     |
 *    80      | vbe_mode          |
 *    82      | vbe_interface_seg |
 *    84      | vbe_interface_off |
 *    86      | vbe_interface_len |
 *            +-------------------+
 *
 * The first longword indicates the presence and validity of other fields in the Multiboot information
 * structure. All as-yet-undefined bits must be set to zero by the boot loader. Any set bits that the
 * operating system does not understand should be ignored. Thus, the ‘flags’ field also functions as a
 * version indicator, allowing the Multiboot information structure to be expanded in the future without
 * breaking anything.
 */
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
	/*
	 * If bit 0 in the ‘flags’ word is set, then the ‘mem_*’ fields are valid. ‘mem_lower’
	 * and ‘mem_upper’ indicate the amount of lower and upper memory, respectively, in kilobytes.
	 * Lower memory starts at address 0, and upper memory starts at address 1 megabyte. The
	 * maximum possible value for lower memory is 640 kilobytes. The value returned for upper
	 * memory is maximally the address of the first upper memory hole minus 1 megabyte. It is
	 * not guaranteed to be this value.
	 */
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

	/*
	 * If bit 1 in the ‘flags’ word is set, then the ‘boot_device’ field is valid, and indicates
	 * which bios disk device the boot loader loaded the OS image from. If the OS image was not
	 * loaded from a bios disk, then this field must not be present (bit 3 must be clear). The
	 * operating system may use this field as a hint for determining its own root device, but is
	 * not required to. The ‘boot_device’ field is laid out in four one-byte subfields as follows:
	 *
	 * +-------+-------+-------+-------+
	 * | part3 | part2 | part1 | drive |
	 * +-------+-------+-------+-------+
	 * The first byte contains the bios drive number as understood by the bios INT 0x13 low-level
	 * disk interface: e.g. 0x00 for the first floppy disk or 0x80 for the first hard disk.
	 *
	 * The three remaining bytes specify the boot partition. ‘part1’ specifies the top-level
	 * partition number, ‘part2’ specifies a sub-partition in the top-level partition, etc.
	 * Partition numbers always start from zero. Unused partition bytes must be set to 0xFF.
	 * For example, if the disk is partitioned using a simple one-level DOS partitioning scheme,
	 * then ‘part1’ contains the DOS partition number, and ‘part2’ and ‘part3’ are both 0xFF. As
	 * another example, if a disk is partitioned first into DOS partitions, and then one of those
	 * DOS partitions is subdivided into several BSD partitions using BSD's disklabel strategy,
	 * then ‘part1’ contains the DOS partition number, ‘part2’ contains the BSD sub-partition
	 * within that DOS partition, and ‘part3’ is 0xFF.
	 *
	 * DOS extended partitions are indicated as partition numbers starting from 4 and increasing,
	 * rather than as nested sub-partitions, even though the underlying disk layout of extended
	 * partitions is hierarchical in nature. For example, if the boot loader boots from the second
	 * extended partition on a disk partitioned in conventional DOS style, then ‘part1’ will be 5,
	 * and ‘part2’ and ‘part3’ will both be 0xFF.
	 */
	if (mbt->flags & MULTIBOOT_INFO_BOOTDEV) {
		if (flags & MB_DEV) {
			printf ("Boot device: [0x%X]\n", (unsigned int)(mbt->boot_device));
			printf (" Drive: [0x%X]\n",
				(unsigned int)(mbt->boot_device & 0xFF000000)>>24);
			printf (" Top-level partition: [0x%X]\n",
				(unsigned int)(mbt->boot_device & 0x00FF0000)>>16);
			printf (" Sub-level partition: [0x%X]\n",
				(unsigned int)(mbt->boot_device & 0x0000FF00)>>8);
			printf (" Sub-sub-level partition: [0x%X]\n",
				(unsigned int)(mbt->boot_device & 0x000000FF));
		}
	}

	/*
	 * If bit 2 of the ‘flags’ longword is set, the ‘cmdline’ field is valid, and contains the
	 * physical address of the command line to be passed to the kernel. The command line is a
	 * normal C-style zero-terminated string.
	 */
	if (mbt->flags & MULTIBOOT_INFO_CMDLINE) {
		if (flags & MB_CMD)
			printf ("Command line: [%s]\n", (char *)(mbt->cmdline));
	}

	/*
	 * If bit 3 of the ‘flags’ is set, then the ‘mods’ fields indicate to the kernel what boot
	 * modules were loaded along with the kernel image, and where they can be found. ‘mods_count’
	 * contains the number of modules loaded; ‘mods_addr’ contains the physical address of the
	 * first module structure. ‘mods_count’ may be zero, indicating no boot modules were loaded,
	 * even if bit 1 of ‘flags’ is set. Each module structure is formatted as follows:
	 *
	 *	   +-------------------+
	 * 0       | mod_start         |
	 * 4       | mod_end           |
	 *	   +-------------------+
	 * 8       | string            |
	 *	   +-------------------+
	 * 12      | reserved (0)      |
	 *	   +-------------------+
	 *
	 * The first two fields contain the start and end addresses of the boot module itself. The
	 * ‘string’ field provides an arbitrary string to be associated with that particular boot
	 * module; it is a zero-terminated ASCII string, just like the kernel command line. The
	 * ‘string’ field may be 0 if there is no string associated with the module. Typically the
	 * string might be a command line (e.g. if the operating system treats boot modules as
	 * executable programs), or a pathname (e.g. if the operating system treats boot modules as
	 * files in a file system), but its exact use is specific to the operating system. The
	 * ‘reserved’ field must be set to 0 by the boot loader and ignored by the operating system.
	 */
	if (mbt->flags & MULTIBOOT_INFO_MODS) {
		if (flags & MB_MODS)
			printf ("Modules count: [0x%X]\n", (unsigned int)(mbt->mods_count));
	}
	/*
	 * Caution: Bits 4 & 5 are mutually exclusive.
	 *
	 * If bit 4 in the ‘flags’ word is set, then the following fields in the Multiboot
	 * information structure starting at byte 28 are valid:
	 *
	 *            +-------------------+
	 *    28      | tabsize           |
	 *    32      | strsize           |
	 *    36      | addr              |
	 *    40      | reserved (0)      |
	 *            +-------------------+
	 *
	 * These indicate where the symbol table from an a.out kernel image can be found. ‘addr’
	 * is the physical address of the size (4-byte unsigned long) of an array of a.out format
	 * nlist structures, followed immediately by the array itself, then the size (4-byte unsigned
	 * long) of a set of zero-terminated ascii strings (plus sizeof(unsigned long) in this case),
	 * and finally the set of strings itself. ‘tabsize’ is equal to its size parameter (found at
	 * the beginning of the symbol section), and ‘strsize’ is equal to its size parameter (found
	 * at the beginning of the string section) of the following string table to which the symbol
	 * table refers. Note that ‘tabsize’ may be 0, indicating no symbols, even if bit 4 in the
	 * ‘flags’ word is set.
	 */
	if (mbt->flags & MULTIBOOT_INFO_AOUT_SYMS) {
		puts ("AOUT kernel, this part need more work");
	}
	/*
	 * Caution: Bits 4 & 5 are mutually exclusive.
	 *
	 * If bit 5 in the ‘flags’ word is set, then the following fields in the Multiboot information
	 * structure starting at byte 28 are valid:
	 *
	 *            +-------------------+
	 *    28      | num               |
	 *    32      | size              |
	 *    36      | addr              |
	 *    40      | shndx             |
	 *            +-------------------+
	 *
	 * These indicate where the section header table from an ELF kernel is, the size of each entry,
	 * number of entries, and the string table used as the index of names. They correspond to the
	 * ‘shdr_*’ entries (‘shdr_num’, etc.) in the Executable and Linkable Format (elf) specification
	 * in the program header. All sections are loaded, and the physical address fields of the elf
	 * section header then refer to where the sections are in memory (refer to the i386 elf documentation
	 * for details as to how to read the section header(s)). Note that ‘shdr_num’ may be 0, indicating no
	 * symbols, even if bit 5 in the ‘flags’ word is set.
	 */
	if (mbt->flags & MULTIBOOT_INFO_ELF_SHDR) {
		puts ("ELF kernel, this part needs more work");
	}
	/*
	 * If bit 6 in the ‘flags’ word is set, then the ‘mmap_*’ fields are valid, and indicate the address
	 * and length of a buffer containing a memory map of the machine provided by the bios. ‘mmap_addr’ is
	 * the address, and ‘mmap_length’ is the total size of the buffer. The buffer consists of one or more
	 * of the following size/structure pairs (‘size’ is really used for skipping to the next pair):
	 *
	 *            +-------------------+
	 *    -4      | size              |
	 *            +-------------------+
	 *    0       | base_addr         |
	 *    8       | length            |
	 *    16      | type              |
	 *            +-------------------+
	 *
	 * where ‘size’ is the size of the associated structure in bytes, which can be greater than the
	 * minimum of 20 bytes. ‘base_addr’ is the starting address. ‘length’ is the size of the memory
	 * region in bytes. ‘type’ is the variety of address range represented, where a value of 1
	 * indicates available ram, and all other values currently indicated a reserved area.
	 *
	 * The map provided is guaranteed to list all standard ram that should be available for normal use.
	 */
	if (mbt->flags & MULTIBOOT_INFO_MEM_MAP) {
		char *mtype[5] = {"Available","Reserved","ACPI reclaimable","NVS","Bad RAM"};
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
				printf (" sz:%lu bytes, addr:0x%llx, len:0x%llx (%llu %s), type:0x%lx (%s)\n",
				       	mmap->size,
					mmap->addr,
					mmap->len,
					mmap->len>>10>0?(mmap->len>>20>0?mmap->len>>20:mmap->len>>10):mmap->len,
					mmap->len>>10>0?(mmap->len>>20>0?"Mb":"Kb"):"B",
					mmap->type,
					mtype[mmap->type - 1]);
			// next mmap, use size of item, plus sizeof size itself
			mmap = (multiboot_mmap *)((unsigned int)mmap + (unsigned int)(mmap->size) + sizeof(uint32_t));
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
	/*
	 * If bit 7 in the ‘flags’ is set, then the ‘drives_*’ fields are valid, and indicate the address of
	 * the physical address of the first drive structure and the size of drive structures. ‘drives_addr’
	 * is the address, and ‘drives_length’ is the total size of drive structures. Note that ‘drives_length’
	 * may be zero. Each drive structure is formatted as follows:
	 *
	 *            +-------------------+
	 *    0       | size              |
	 *            +-------------------+
	 *    4       | drive_number      |
	 *            +-------------------+
	 *    5       | drive_mode        |
	 *            +-------------------+
	 *    6       | drive_cylinders   |
	 *    8       | drive_heads       |
	 *    9       | drive_sectors     |
	 *            +-------------------+
	 *    10 - xx | drive_ports       |
	 *            +-------------------+
	 *
	 * The ‘size’ field specifies the size of this structure. The size varies, depending on the number
	 * of ports. Note that the size may not be equal to (10 + 2 * the number of ports), because of an alignment.
	 *
	 * The ‘drive_number’ field contains the BIOS drive number. The ‘drive_mode’ field represents the
	 * access mode used by the boot loader. Currently, the following modes are defined:
	 *
	 * ‘0’
	 *    CHS mode (traditional cylinder/head/sector addressing mode).
	 * ‘1’
	 *    LBA mode (Logical Block Addressing mode).
	 *
	 * The three fields, ‘drive_cylinders’, ‘drive_heads’ and ‘drive_sectors’, indicate the geometry of
	 * the drive detected by the bios. ‘drive_cylinders’ contains the number of the cylinders. ‘drive_heads’
	 * contains the number of the heads. ‘drive_sectors’ contains the number of the sectors per track.
	 *
	 * The ‘drive_ports’ field contains the array of the I/O ports used for the drive in the bios code. The
	 * array consists of zero or more unsigned two-bytes integers, and is terminated with zero. Note that
	 * the array may contain any number of I/O ports that are not related to the drive actually (such as
	 * dma controller's ports).
	 */
	if (mbt->flags & MULTIBOOT_INFO_DRIVE_INFO) {
		if (flags & MB_DRIVE)
			printf ("Drive info: [0x%X]\n", (unsigned int)(mbt->drives_length));
	}
	/*
	 * If bit 8 in the ‘flags’ is set, then the ‘config_table’ field is valid, and indicates the address
	 * of the rom configuration table returned by the GET CONFIGURATION bios call. If the bios call fails,
	 * then the size of the table must be zero.
	 */
	if (mbt->flags & MULTIBOOT_INFO_CONFIG_TABLE) {
		if (flags & MB_CONFIG)
			printf ("Config table: [0x%X]\n", (unsigned int)(mbt->config_table));
	}
	/*
	 * If bit 9 in the ‘flags’ is set, the ‘boot_loader_name’ field is valid, and contains the physical
	 * address of the name of a boot loader booting the kernel. The name is a normal C-style
	 * zero-terminated string.
	 */
	if (mbt->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME) {
		if (flags & MB_DEV)
			printf ("Bootloader: [%s]\n", (const char *)(mbt->boot_loader_name));
	}
	/*
	 * If bit 10 in the ‘flags’ is set, the ‘apm_table’ field is valid, and contains the physical
	 * address of an apm table defined as below:
	 *
	 *            +----------------------+
	 *    0       | version              |
	 *    2       | cseg                 |
	 *    4       | offset               |
	 *    8       | cseg_16              |
	 *    10      | dseg                 |
	 *    12      | flags                |
	 *    14      | cseg_len             |
	 *    16      | cseg_16_len          |
	 *    18      | dseg_len             |
	 *            +----------------------+
	 *
	 * The fields ‘version’, ‘cseg’, ‘offset’, ‘cseg_16’, ‘dseg’, ‘flags’, ‘cseg_len’, ‘cseg_16_len’,
	 * ‘dseg_len’ indicate the version number, the protected mode 32-bit code segment, the offset of
	 * the entry point, the protected mode 16-bit code segment, the protected mode 16-bit data segment,
	 * the flags, the length of the protected mode 32-bit code segment, the length of the protected
	 * mode 16-bit code segment, and the length of the protected mode 16-bit data segment, respectively.
	 * Only the field ‘offset’ is 4 bytes, and the others are 2 bytes. See Advanced Power Management
	 * (APM) BIOS Interface Specification, for more information.
	 */
	if (mbt->flags & MULTIBOOT_INFO_APM_TABLE) {
		if (flags & MB_APM)
			printf ("APM table: [0x%X]\n", (unsigned int)(mbt->apm_table));
	}
	/*
	 * If bit 11 in the ‘flags’ is set, the graphics table is available. This must only be done if
	 * the kernel has indicated in the ‘Multiboot Header’ that it accepts a graphics mode.
	 *
	 * The fields ‘vbe_control_info’ and ‘vbe_mode_info’ contain the physical addresses of vbe
	 * control information returned by the vbe Function 00h and vbe mode information returned by
	 * the vbe Function 01h, respectively.
	 *
	 * The field ‘vbe_mode’ indicates current video mode in the format specified in vbe 3.0.
	 *
	 * The rest fields ‘vbe_interface_seg’, ‘vbe_interface_off’, and ‘vbe_interface_len’ contain
	 * the table of a protected mode interface defined in vbe 2.0+. If this information is not
	 * available, those fields contain zero. Note that vbe 3.0 defines another protected mode
	 * interface which is incompatible with the old one. If you want to use the new protected
	 * mode interface, you will have to find the table yourself.
	 *
	 * The fields for the graphics table are designed for vbe, but Multiboot boot loaders may
	 * simulate vbe on non-vbe modes, as if they were vbe modes.
	 */
	if (mbt->flags & MULTIBOOT_INFO_VBE_INFO) {
		if (flags & MB_VBE)
			printf ("Graphics table %s\n","available");
	}
}

