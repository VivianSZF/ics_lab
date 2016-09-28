#include "cpu/exec/template-start.h"

#define instr je

static void do_execute(){
	if(cpu.ZF==1){ 
		cpu.eip+=op_src->val;
		print_asm("je"str(SUFFIX) " %x",cpu.eip+1+len);
	}
	else
	{
		print_asm("je"str(SUFFIX) " %x",cpu.eip+op_src->val+1+len);
	}
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
