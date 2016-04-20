/*
 * Interrupt Service Requests
 */
.intel_syntax noprefix

.extern fault_handler

.globl	isr0
.globl	isr1
.globl	isr2
.globl	isr3
.globl	isr4
.globl	isr5
.globl	isr6
.globl	isr7
.globl	isr8
.globl	isr9
.globl	isr10
.globl	isr11
.globl	isr12
.globl	isr13
.globl	isr14
.globl	isr15
.globl	isr16
.globl	isr17
.globl	isr18
.globl	isr19
.globl	isr20
.globl	isr21
.globl	isr22
.globl	isr23
.globl	isr24
.globl	isr25
.globl	isr26
.globl	isr27
.globl	isr28
.globl	isr29
.globl	isr30
.globl	isr31

isr0:
	cli
	push 0 /* dummy errorcode */
	push 0 /* int_no */
	jmp isr_common

isr1:
	cli
	push 0 /* dummy errorcode */
	push 1 /* int_no */
	jmp isr_common
isr2:
	cli
	push 0 /* dummy errorcode */
	push 2 /* int_no */
	jmp isr_common
isr3:
	cli
	push 0 /* dummy errorcode */
	push 3 /* int_no */
	jmp isr_common
isr4:
	cli
	push 0 /* dummy errorcode */
	push 4 /* int_no */
	jmp isr_common
isr5:
	cli
	push 0 /* dummy errorcode */
	push 5 /* int_no */
	jmp isr_common
isr6:
	cli
	push 0 /* dummy errorcode */
	push 6 /* int_no */
	jmp isr_common
isr7:
	cli
	push 0 /* dummy errorcode */
	push 7 /* int_no */
	jmp isr_common
isr8:
	cli

	push 8 /* int_no */
	jmp isr_common
isr9:
	cli
	push 0 /* dummy errorcode */
	push 9 /* int_no */
	jmp isr_common
isr10:
	cli

	push 10 /* int_no */
	jmp isr_common
isr11:
	cli

	push 11 /* int_no */
	jmp isr_common
isr12:
	cli

	push 12 /* int_no */
	jmp isr_common
isr13:
	cli

	push 13 /* int_no */
	jmp isr_common
isr14:
	cli

	push 14 /* int_no */
	jmp isr_common
isr15:
	cli
	push 0 /* dummy errorcode */
	push 15 /* int_no */
	jmp isr_common
isr16:
	cli
	push 0 /* dummy errorcode */
	push 16 /* int_no */
	jmp isr_common
isr17:
	cli
	push 0 /* dummy errorcode */
	push 17 /* int_no */
	jmp isr_common
isr18:
	cli
	push 0 /* dummy errorcode */
	push 18 /* int_no */
	jmp isr_common
isr19:
	cli
	push 0 /* dummy errorcode */
	push 19 /* int_no */
	jmp isr_common
isr20:
	cli
	push 0 /* dummy errorcode */
	push 20 /* int_no */
	jmp isr_common
isr21:
	cli
	push 0 /* dummy errorcode */
	push 21 /* int_no */
	jmp isr_common
isr22:
	cli
	push 0 /* dummy errorcode */
	push 22 /* int_no */
	jmp isr_common
isr23:
	cli
	push 0 /* dummy errorcode */
	push 23 /* int_no */
	jmp isr_common
isr24:
	cli
	push 0 /* dummy errorcode */
	push 24 /* int_no */
	jmp isr_common
isr25:
	cli
	push 0 /* dummy errorcode */
	push 25 /* int_no */
	jmp isr_common
isr26:
	cli
	push 0 /* dummy errorcode */
	push 26 /* int_no */
	jmp isr_common
isr27:
	cli
	push 0 /* dummy errorcode */
	push 27 /* int_no */
	jmp isr_common
isr28:
	cli
	push 0 /* dummy errorcode */
	push 28 /* int_no */
	jmp isr_common
isr29:
	cli
	push 0 /* dummy errorcode */
	push 29 /* int_no */
	jmp isr_common
isr30:
	cli
	push 0 /* dummy errorcode */
	push 30 /* int_no */
	jmp isr_common
isr31:
	cli
	push 0 /* dummy errorcode */
	push 31 /* int_no */
	jmp isr_common
isr_common:
	pusha			/* we are building a struct regs on the stack */
	push ds			/* the cpu has pushed eip,cd,eflags,esp and ss for us */
	push es			/* in the isr# we have pushed erro and int no */
	push fs
	push gs

	mov ax,0x10		/* kernel data segment */
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax

	mov eax,esp		/* Finally: the function wants a pointer to struct */
	push eax		/* So we push the address of the stack! */
	call fault_handler	/* kernel code segment selector is set in idt table entry */
	pop eax			/* stack cleanup below */

	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp,8		/* errorcode and int_no */
	iret			/* finally: iret pops eip,cs,eflags,esp and ss */
