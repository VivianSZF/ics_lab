#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	int len=instr_len();
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip+len);
	cpu.eip+=op_src->val;
	print_asm("call %x",op_src->val);
	print_asm("call %x",cpu.eip+len+1);
}

make_instr_helper(i);
make_instr_helper(rm);

#include "cpu/exec/template-end.h"
