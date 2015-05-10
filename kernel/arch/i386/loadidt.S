
.intel_syntax noprefix
gdtr_limit:	.word	0
gdtr_base:	.long	0
.globl loadidt

loadidt:
	mov eax, [esp + 4]
	mov [gdtr_base], eax
	mov ax, [esp + 8]
	mov [gdtr_limit], ax
	
	cli

	lidt [gdtr_limit]

	sti

	ret

