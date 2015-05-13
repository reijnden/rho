/*
 * Interrupt Request Requests
 */
.intel_syntax noprefix

.extern irq_handler

.globl	irq0
.globl	irq1
.globl	irq2
.globl	irq3
.globl	irq4
.globl	irq5
.globl	irq6
.globl	irq7
.globl	irq8
.globl	irq9
.globl	irq10
.globl	irq11
.globl	irq12
.globl	irq13
.globl	irq14
.globl	irq15

irq0:
	cli
	push 0 /* dummy errorcode */
	push 32 /* int_no */
	jmp irq_common

irq1:
	cli
	push 0 /* dummy errorcode */
	push 33 /* int_no */
	jmp irq_common
irq2:
	cli
	push 0 /* dummy errorcode */
	push 34 /* int_no */
	jmp irq_common
irq3:
	cli
	push 0 /* dummy errorcode */
	push 35 /* int_no */
	jmp irq_common
irq4:
	cli
	push 0 /* dummy errorcode */
	push 36 /* int_no */
	jmp irq_common
irq5:
	cli
	push 0 /* dummy errorcode */
	push 37 /* int_no */
	jmp irq_common
irq6:
	cli
	push 0 /* dummy errorcode */
	push 38 /* int_no */
	jmp irq_common
irq7:
	cli
	push 0 /* dummy errorcode */
	push 39 /* int_no */
	jmp irq_common
irq8:
	cli
	push 0 /* dummy errorcode */
	push 40 /* int_no */
	jmp irq_common
irq9:
	cli
	push 0 /* dummy errorcode */
	push 41 /* int_no */
	jmp irq_common
irq10:
	cli
	push 0 /* dummy errorcode */
	push 42 /* int_no */
	jmp irq_common
irq11:
	cli

	push 0 /* dummy errorcode */
	push 43 /* int_no */
	jmp irq_common
irq12:
	cli
	push 0 /* dummy errorcode */
	push 44 /* int_no */
	jmp irq_common
irq13:
	cli

	push 0 /* dummy errorcode */
	push 45 /* int_no */
	jmp irq_common
irq14:
	cli
	push 0 /* dummy errorcode */
	push 46 /* int_no */
	jmp irq_common
irq15:
	cli
	push 0 /* dummy errorcode */
	push 47 /* int_no */
	jmp irq_common

irq_common:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax,0x10	/* kernel data segment */
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov eax,esp	/* push the stack pointer, it contains cpu state for irq_handler*/
	push eax
	/*mov eax,irq_handler
	call eax*/
	call irq_handler
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp,8	/* errorcode and int_no */
	sti
	iret
