#include "cpu/exec/template-start.h"

#define instr je

/*make_helper(concat(je_si_,SUFFIX)){
	DATA_TYPE_S imm=instr_fetch(eip+1,DATA_BYTE);
	print_asm("je" str(SUFFIX) " %x",eip+1+imm+DATA_BYTE);
	if(cpu.ZF==1) cpu.eip+=imm;
	return DATA_BYTE+1;
}
*/
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
