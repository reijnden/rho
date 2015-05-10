
.intel_syntax noprefix
gdtr_limit:	.word	0
gdtr_base:	.long	0
.globl loadgdt

/*
gdtr	dw 0
	dd 0
global loadgdt
*/
loadgdt:
	mov eax, [esp + 4]
	mov [gdtr_base], eax
	mov ax, [esp + 8]
	mov [gdtr_limit], ax
	
	cli

	lgdt [gdtr_limit]

	ret
/*
 * 0x08 is the code segment in gdt
 */
	jmp 0x08:reload_cs
reload_cs:
/*
 * 0x10 is the data segment in gdt
 */
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax


	sti

	ret

