#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute(){
	REG(R_EAX)=MEM_R(REG(R_ESI));
	if(cpu.DF==0)
		cpu.esi+=DATA_BYTE;
	else
		cpu.esi-=DATA_BYTE;
	print_asm_template2();
}

make_instr_helper(n);

#include "cpu/exec/template-end.h"

