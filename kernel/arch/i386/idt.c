#include <stdint.h>
#include <kernel/gdt.h>
#include <stdio.h>

#define IDT_ENTRIES	256

static struct idt_entry idt[IDT_ENTRIES];

static unsigned char *int_msg[] = {
	(unsigned char *)"Division by zero",
	(unsigned char *)"Debugger",
	(unsigned char *)"Non Maskable Interrupt",
	(unsigned char *)"Breakpoint",
	(unsigned char *)"Overflow",
	(unsigned char *)"Out of Bounds",
	(unsigned char *)"Invalid opcode",
	(unsigned char *)"Coprocessor not available",
	(unsigned char *)"Double fault",
	(unsigned char *)"Coprocessor Segment Overrun",
	(unsigned char *)"Invalid Task State Segment",
	(unsigned char *)"Segment not present",
	(unsigned char *)"Stack Fault",
	(unsigned char *)"General Protection Fault",
	(unsigned char *)"Page fault",
	(unsigned char *)"0x0f reserved",
	(unsigned char *)"Math Fault",
	(unsigned char *)"Alignment Check",
	(unsigned char *)"Machine Check",
	(unsigned char *)"SIMD Floating-Point Exception",
	(unsigned char *)"0x14 reserved",
	(unsigned char *)"0x15 reserved",
	(unsigned char *)"0x16 reserved",
	(unsigned char *)"0x17 reserved",
	(unsigned char *)"0x18 reserved",
	(unsigned char *)"0x19 reserved",
	(unsigned char *)"0x1a reserved",
	(unsigned char *)"0x1b reserved",
	(unsigned char *)"0x1c reserved",
	(unsigned char *)"0x1d reserved",
	(unsigned char *)"0x1e reserved",
	(unsigned char *)"0x2f reserved"
};

static void idt_set_gate(int num, uint32_t base, uint16_t selector, uint8_t type) {
	idt[num].base_low = (base & 0xFFFF);		// copy lower 2 bytes 
	idt[num].base_high = (base >> 16 ) & 0xFFFF;	// shift 2 bytes , copy lower 2 byte

	idt[num].selector = selector;
	idt[num].type = type;	// 0x8e

	idt[num].zero = 0x00;
}

void idt_install() {
	static struct idt_ptr id;
	id.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
	id.base =(uint32_t) &idt;

	idt_set_gate(0,(uint32_t)isr0,0x08,0x8e);
	idt_set_gate(1,(uint32_t)isr1,0x08,0x8e);
	idt_set_gate(2,(uint32_t)isr2,0x08,0x8e);
	idt_set_gate(3,(uint32_t)isr3,0x08,0x8e);
	idt_set_gate(4,(uint32_t)isr4,0x08,0x8e);
	idt_set_gate(5,(uint32_t)isr5,0x08,0x8e);
	idt_set_gate(6,(uint32_t)isr6,0x08,0x8e);
	idt_set_gate(7,(uint32_t)isr7,0x08,0x8e);
	idt_set_gate(8,(uint32_t)isr8,0x08,0x8e);
	idt_set_gate(9,(uint32_t)isr9,0x08,0x8e);
	idt_set_gate(10,(uint32_t)isr10,0x08,0x8e);
	idt_set_gate(11,(uint32_t)isr11,0x08,0x8e);
	idt_set_gate(12,(uint32_t)isr12,0x08,0x8e);
	idt_set_gate(13,(uint32_t)isr13,0x08,0x8e);
	idt_set_gate(14,(uint32_t)isr14,0x08,0x8e);
	idt_set_gate(15,(uint32_t)isr15,0x08,0x8e);
	idt_set_gate(16,(uint32_t)isr16,0x08,0x8e);
	idt_set_gate(17,(uint32_t)isr17,0x08,0x8e);
	idt_set_gate(18,(uint32_t)isr18,0x08,0x8e);
	idt_set_gate(19,(uint32_t)isr19,0x08,0x8e);
	idt_set_gate(20,(uint32_t)isr20,0x08,0x8e);
	idt_set_gate(21,(uint32_t)isr21,0x08,0x8e);
	idt_set_gate(22,(uint32_t)isr22,0x08,0x8e);
	idt_set_gate(23,(uint32_t)isr23,0x08,0x8e);
	idt_set_gate(24,(uint32_t)isr24,0x08,0x8e);
	idt_set_gate(25,(uint32_t)isr25,0x08,0x8e);
	idt_set_gate(26,(uint32_t)isr26,0x08,0x8e);
	idt_set_gate(27,(uint32_t)isr27,0x08,0x8e);
	idt_set_gate(28,(uint32_t)isr28,0x08,0x8e);
	idt_set_gate(29,(uint32_t)isr29,0x08,0x8e);
	idt_set_gate(30,(uint32_t)isr30,0x08,0x8e);
	idt_set_gate(31,(uint32_t)isr31,0x08,0x8e);

	asm ( "lidtl %0" : : "m" (id) );
}

static void coredump(struct regs *r) {
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

void interrupt_handler(struct regs *r){
	switch (r->int_no) {
		case 0:
			printf ("Interrupt [%i:%i][%s]!\n",r->int_no,r->err_code,int_msg[r->int_no]);
			coredump(r);
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
		case 31:
			printf ("Interrupt [%i:%i][%s]! Halting system\n",r->int_no,r->err_code,int_msg[r->int_no]);
			while ( 1 ) { }
			__builtin_unreachable();
			break;
		default:
			printf("Unknown fault! Halting system\n");
			while ( 1 ) { }
			__builtin_unreachable();
			break;
	}
}

