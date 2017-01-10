#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	int len=getinstr_len();
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip+len,S_SS);
	if(op_src->type==OP_TYPE_IMM){
		cpu.eip+=op_src->val;
		print_asm("call 0x%x",cpu.eip+len+1);
	}
	else{
		cpu.eip=op_src->val-len-1;
		print_asm_template1();
	}
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
make_instr_helper(si)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
