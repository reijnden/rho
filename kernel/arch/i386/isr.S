/*
 * Interrup Service Routine Wrapper
 */

.globl	isr_wrapper
.align	4

isr_wrapper:
	pusha
	cld	/* sysV ABI sais clear df */
	/*call interrupt_handler*/
	popa
	iret
