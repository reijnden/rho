
#ifndef MULTIBOOT_HEADER
#define MULTIBOOT_HEADER 1

/* The magic field  */
#define MULTIBOOT_MAGIC			0x2BADB002

/* Flags  */

/* lower/upper memory */
#define MULTIBOOT_INFO_MEMORY			0x00000001
/* boot device */
#define MULTIBOOT_INFO_BOOTDEV			0x00000002
/* cmd-line */
#define MULTIBOOT_INFO_CMDLINE			0x00000004
/* modules */
#define MULTIBOOT_INFO_MODS			0x00000008
/* syms */
#define MULTIBOOT_INFO_AOUT_SYMS		0x00000010
#define MULTIBOOT_INFO_ELF_SHDR			0X00000020
/* memory map */
#define MULTIBOOT_INFO_MEM_MAP			0x00000040
/* drive info */
#define MULTIBOOT_INFO_DRIVE_INFO		0x00000080
/* config table  */
#define MULTIBOOT_INFO_CONFIG_TABLE		0x00000100
/* loader name  */
#define MULTIBOOT_INFO_BOOT_LOADER_NAME		0x00000200
/* APM table */
#define MULTIBOOT_INFO_APM_TABLE		0x00000400
/* video */
#define MULTIBOOT_INFO_VBE_INFO		        0x00000800

typedef struct
{
  uint32_t flags;

  uint32_t mem_lower;
  uint32_t mem_upper;
  
  uint32_t boot_device;
  
  uint32_t cmdline;
  
  uint32_t mods_count;
  uint32_t mods_addr;

  uint32_t syms1; // I don't use symbol tables for elf/aout yet...
  uint32_t syms2;
  uint32_t syms3;
  uint32_t syms4;

  uint32_t mmap_length;
  uint32_t mmap_addr;

  uint32_t drives_length;
  uint32_t drives_addr;

  uint32_t config_table;

  uint32_t boot_loader_name;

  uint32_t apm_table;

  uint32_t vbe_control_info;
  uint32_t vbe_mode_info;
  uint16_t vbe_mode;
  uint16_t vbe_interface_seg;
  uint16_t vbe_interface_off;
  uint16_t vbe_interface_len;
} multiboot_info;

typedef struct
{
  uint32_t size;
  uint64_t addr;
  uint64_t len;
#define MULTIBOOT_MEMORY_AVAILABLE		1
#define MULTIBOOT_MEMORY_RESERVED		2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5
  uint32_t type;
} multiboot_mmap;

void boot_info(multiboot_info *mbt,uint16_t flags);

#define MB_MEMORY	0x0001
#define MB_DEV		0x0002
#define MB_CMD		0x0004
#define MB_MODS		0x0008
#define MB_DRIVE	0x0010
#define MB_CONFIG	0x0020
#define MB_APM		0x0040
#define MB_VBE		0x0080

#endif /* ! MULTIBOOT_HEADER */
