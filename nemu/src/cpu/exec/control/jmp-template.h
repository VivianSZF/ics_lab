#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute(){
	int len=getinstr_len();
	if(op_src->type==OP_TYPE_IMM){
		cpu.eip+=(DATA_TYPE_S)op_src->val;
		print_asm("jmp"str(SUFFIX) " %x",cpu.eip+len+1);
		}
	else{
		cpu.eip=op_src->val-len-1;
		print_asm_template1();
	}
}

make_instr_helper(si)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
