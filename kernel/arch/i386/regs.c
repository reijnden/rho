#include <kernel/regs.h>
#include <stdio.h>

void coredump(struct regs *r) {
	printf ("EIP:0x%x",r->eip);
	printf (",EAX:0x%x",r->eax);
	printf (",EBX:0x%x",r->ebx);
	printf (",ECX:0x%x",r->ecx);
	printf (",EDX:0x%x",r->edx);
	printf (",ESP:0x%x",r->esp);
	printf (",EBP:0x%x",r->ebp);
	printf (",ESI:0x%x",r->esi);
	printf (",EDI:0x%x",r->edi);
	printf ("\nCS:0x%x",r->cs);
	printf (",DS:0x%x",r->ds);
	printf (",ES:0x%x",r->es);
	printf (",FS:0x%x",r->fs);
	printf (",GS:0x%x",r->gs);
	printf (",SS:0x%x",r->ss);
	printf ("\nEFLAGS:0x%x\n",r->eflags);
}

