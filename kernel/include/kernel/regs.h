#ifndef _KERNEL_REGS_H
#define _KERNEL_REGS_H


struct regs {
	unsigned int gs,fs,es,ds;
	unsigned int edi,esi,ebp,esp,ebx,edx,ecx,eax;
	unsigned int int_no,err_code;
	unsigned int eip,cs,eflags,useresp,ss;
};

void coredump(struct regs *r);

#endif
