#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	int len=instr_len();
	cpu.esp-=DATA_BYTE;
	swaddr_write(cpu.esp,4,cpu.eip+len);
	DATA_TYPE_S displacement=op_src->val;
	cpu.eip+=displacement;
	print_asm("call %x",cpu.eip+len+1);
}

make_instr_helper(i);
make_instr_helper(rm);

#include "cpu/exec/template-end.h"
