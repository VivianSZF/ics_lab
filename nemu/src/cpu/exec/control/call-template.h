#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	int len=instr_len();
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip+len);
	cpu.eip+=op_src->val;
	print_asm("call %x",cpu.eip+len+1);
}

/*
make_helper(concat(call_i_,SUFFIX))
{
	int len=concat(decode_i_,SUFFIX)(eip+1);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip+len);
	cpu.eip+=op_src->val;
	print_asm("call %x",cpu.eip+len+1);
	return len+1;
}
*/
make_instr_helper(i);


#include "cpu/exec/template-end.h"
