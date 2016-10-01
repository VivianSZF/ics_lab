#include "cpu/exec/template-start.h"

#define instr cwtl

static void do_execute(){
	if((cpu.eax>>15)&1)
		cpu.edx|=0xffff;
	else
		cpu.edx&=0x0000;//????????
	print_asm("cwtl");
}

make_instr_helper(n);

#include "cpu/exec/template-end.h"  
