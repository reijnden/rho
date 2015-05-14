#include <kernel/regs.h>
#include <stdio.h>

void coredump(struct regs *r) {
	printf ("EIP:0x%lx",r->eip);
	printf (",EAX:0x%lx",r->eax);
	printf (",EBX:0x%lx",r->ebx);
	printf (",ECX:0x%lx",r->ecx);
	printf (",EDX:0x%lx",r->edx);
	printf (",ESP:0x%lx",r->esp);
	printf (",EBP:0x%lx",r->ebp);
	printf (",ESI:0x%lx",r->esi);
	printf (",EDI:0x%lx",r->edi);
	printf ("\nCS:0x%lx",r->cs);
	printf (",DS:0x%lx",r->ds);
	printf (",ES:0x%lx",r->es);
	printf (",FS:0x%lx",r->fs);
	printf (",GS:0x%lx",r->gs);
	printf (",SS:0x%lx",r->ss);
	printf (",useresp:0x%lx",r->useresp);
	printf ("\nEFLAGS:0x%lx\n",r->eflags);
}

