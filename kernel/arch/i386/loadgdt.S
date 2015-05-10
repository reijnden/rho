
.intel_syntax noprefix
/*
 * 6 bytes gdt
gdtr_limit:	.word	0
gdtr_base:	.long	0
 */

.globl refreshGDT

/*
 * loadgdt(gd.base,gd.limit);
   stack:
	esp:		eip
	esp + 4:	base
	esp + 8:	limit
loadgdt:
	mov eax, [esp + 4]
	mov [gdtr_base], eax
	mov ax, [esp + 8]
	mov [gdtr_limit], ax
	
	cli
	lgdt [gdtr_limit]

 */
refreshGDT:
	cli
	jmp 0x08:reload_cs
reload_cs:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	sti

	ret

