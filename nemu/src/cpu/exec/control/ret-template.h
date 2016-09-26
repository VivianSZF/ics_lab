#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute(){
	if(DATA_BYTE==2){
		cpu.eip=swaddr_read(cpu.esp,2)-1;
		cpu.esp+=DATA_BYTE;
		cpu.esp+=op_src->val;
		cpu.eip=cpu.eip&0xffff;
	}
	else
	{
		cpu.eip=swaddr_read(cpu.esp,4)-1;
		cpu.esp+=4;
		cpu.esp+=op_src->val;
	}
	if(op_src->val==0)
		print_asm("ret");
	else
		print_asm_template1();
}

make_instr_helper(n)
make_instr_helper(i)

#include "cpu/exec/template-end.h"
