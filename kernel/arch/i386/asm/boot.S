# Declare constants used for creating a multiboot header.
.set ALIGN,	1<<0             		# align loaded modules on page boundaries
.set MEMINFO,	1<<1             		# provide memory map
.set VIDEO,	1<<2             		# provide video mode table
.set FLAGS,	ALIGN | MEMINFO | VIDEO 	# this is the Multiboot 'flag' field
.set MAGIC,	0x1BADB002       		# 'magic number' lets bootloader find the header
.set CHECKSUM,	-(MAGIC + FLAGS) 		# checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
# The layout of the Multiboot header must be as follows:
#
# Offset 	Type 	Field Name 	Note
#  0 	u32 	magic 	required
#  4 	u32 	flags 	required
#  8 	u32 	checksum 	required
# 12 	u32 	header_addr 	if flags[16] is set
# 16 	u32 	load_addr 	if flags[16] is set
# 20 	u32 	load_end_addr 	if flags[16] is set
# 24 	u32 	bss_end_addr 	if flags[16] is set
# 28 	u32 	entry_addr 	if flags[16] is set
# 32 	u32 	mode_type 	if flags[2] is set
# 36 	u32 	width 	if flags[2] is set
# 40 	u32 	height 	if flags[2] is set
# 44 	u32 	depth 	if flags[2] is set
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0					# header_addr
.long 0					# load_addr
.long 0					# load_end_addr
.long 0					# bss_end_addr
.long 0					# entry_addr
# Required since flags[2] is set for video mode info
.long 1					# mode_type
.long 80				# width
.long 25				# height
.long 0					# depth

# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
	movl $stack_top, %esp
	push %ebx	# store 32 bit physical address of multiboot_header;
	push %eax	# store bootloader magic number, so we can check it in kernel_early

	# Initialize the core kernel before running the global constructors.
	# The stack contains magic number, multiboot_header
	# So the function can use these, in this order
	call kernel_early

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	pop %eax	# pop the magic number, put multiboot header address up for kernel_main
	# The stack now contains the multiboot_header only
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
	hlt
.Lhang:
	jmp .Lhang
.size _start, . - _start
