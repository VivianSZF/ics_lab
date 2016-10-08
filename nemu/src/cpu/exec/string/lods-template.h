#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute(){
	if(DATA_BYTE==1){
		swaddr_write(cpu.gpr[0]._8[0],DATA_BYTE,cpu.gpr[6]._8[0]);
	}
	else if (DATA_BYTE==2){
		swaddr_write(cpu.gpr[0]._16,DATA_BYTE,cpu.gpr[6]._16);
	}
	else{
		swaddr_write(cpu.eax,DATA_BYTE,cpu.esi);
	}
	if(cpu.DF==0)
		cpu.esi+=DATA_BYTE;
	else
		cpu.esi+=DATA_BYTE;
	print_asm_template2();
}

make_instr_helper(n);

#include "cpu/exec/template-end.h"

