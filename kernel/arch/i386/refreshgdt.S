
.intel_syntax noprefix

.globl refresh_gdt

refresh_gdt:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:reload_cs
reload_cs:
	ret
