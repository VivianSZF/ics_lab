#include "cpu/exec/template-start.h"

#define instr stos

static void do_execute(){
	/*if(DATA_BYTE==1){
		swaddr_write(cpu.edi,DATA_BYTE,cpu.gpr[0]._8[0]);
	}
	else if(DATA_BYTE==2){
		swaddr_write(cpu.edi,DATA_BYTE,cpu.gpr[0]._16);
	}
	else{
		swaddr_write(cpu.edi,DATA_BYTE,cpu.eax);
	}*/
	swaddr_write(reg_l(R_EDI),DATA_BYTE,(DATA_TYPE)reg_l(R_EAX));
	if(cpu.DF==0)
		cpu.edi+=DATA_BYTE;
	else
		cpu.edi-=DATA_BYTE;
	print_asm_template2();
}

make_instr_helper(n);

#include "cpu/exec/template-end.h"
