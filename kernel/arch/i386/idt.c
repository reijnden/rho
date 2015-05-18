#include <stdint.h>
#include <kernel/idt.h>
#include <kernel/regs.h>
#include <stdio.h>

#define IDT_ENTRIES	256
#define IDT_SELECTOR	0x08				/* selector of code segment */
							/* containing fault_handlers */
/*
 * IDT entry type and attribute bitmasks
 */
#define IDT_P		0x80
#define IDT_DPL0	0x0				/* DPL ring levels */
#define IDT_DPL1	0x20
#define IDT_DPL2	0x40
#define IDT_DPL3	0x60
#define IDT_S		0x0				/* Storage segment */
#define IDT_T_TASK	0x5				/* Task gate */
#define IDT_T_INT16	0x6				/* 16bit Interrupt gate */
#define IDT_T_TRAP16	0x7				/* 16bit Trap gate */
#define IDT_T_INT32	0xE				/* 32bit Interrupt gate */
#define IDT_T_TRAP32	0xF				/* 32bit Trap gate */

static struct idt_entry idt[IDT_ENTRIES];

/*
 * Exception messages
 */
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

void idt_set_gate(int num, uint32_t base, uint16_t selector, uint8_t type) {
	idt[num].base_low = (base & 0xFFFF);		/* copy lower 2 bytes */
	idt[num].base_high = (base >> 16 ) & 0xFFFF;	/* shift 2 bytes , copy lower 2 byte */
	idt[num].selector = selector;
	idt[num].type = type;				/* type and attributes */
	idt[num].zero = 0x00;
}

void idt_install() {
	static struct idt_ptr id;
	id.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
	id.base =(uint32_t) &idt;

	idt_set_gate(0,(uint32_t)isr0,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(1,(uint32_t)isr1,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(2,(uint32_t)isr2,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(3,(uint32_t)isr3,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(4,(uint32_t)isr4,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(5,(uint32_t)isr5,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(6,(uint32_t)isr6,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(7,(uint32_t)isr7,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(8,(uint32_t)isr8,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(9,(uint32_t)isr9,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(10,(uint32_t)isr10,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(11,(uint32_t)isr11,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(12,(uint32_t)isr12,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(13,(uint32_t)isr13,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(14,(uint32_t)isr14,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(15,(uint32_t)isr15,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(16,(uint32_t)isr16,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(17,(uint32_t)isr17,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(18,(uint32_t)isr18,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(19,(uint32_t)isr19,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(20,(uint32_t)isr20,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(21,(uint32_t)isr21,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(22,(uint32_t)isr22,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(23,(uint32_t)isr23,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(24,(uint32_t)isr24,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(25,(uint32_t)isr25,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(26,(uint32_t)isr26,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(27,(uint32_t)isr27,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(28,(uint32_t)isr28,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(29,(uint32_t)isr29,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(30,(uint32_t)isr30,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);
	idt_set_gate(31,(uint32_t)isr31,IDT_SELECTOR,IDT_P|IDT_DPL0|IDT_T_INT32);

	asm ( "lidtl %0" : : "m" (id) );
}

static void coredump(struct regs *r) {
	printf ("\n=================Registers===================\n");
	printf ("EIP:0x%lx",r->eip);
	printf (", EAX:0x%lx",r->eax);
	printf (", EBX:0x%lx",r->ebx);
	printf (", ECX:0x%lx",r->ecx);
	printf (", EDX:0x%lx",r->edx);
	printf (", ESP:0x%lx",r->esp);
	printf (", EBP:0x%lx",r->ebp);
	printf (", ESI:0x%lx",r->esi);
	printf (", EDI:0x%lx",r->edi);
	printf ("\nCS:0x%lx",r->cs);
	printf (", DS:0x%lx",r->ds);
	printf (", ES:0x%lx",r->es);
	printf (", FS:0x%lx",r->fs);
	printf (", GS:0x%lx",r->gs);
	printf (", SS:0x%lx",r->ss);
	printf (", UserESP:0x%lx",r->useresp);
	printf ("\nEFLAGS:0x%lx\n",r->eflags);
	printf ("=============================================\n");
}

void fault_handler(struct regs *r){
	switch (r->int_no) {
		case 0:
		case 1:					/* Debugger */
			coredump(r);
			break;
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
			printf ("Interrupt [%li:%li][%s]! Halting system\n",
					r->int_no,r->err_code,int_msg[r->int_no]);
			coredump(r);
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
