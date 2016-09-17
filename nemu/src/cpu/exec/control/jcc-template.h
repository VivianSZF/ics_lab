#include "cpu/exec/template-start.h"

#define instr je

make_helper(concat(je_si_,SUFFIX)){
	DATA_TYPE_S imm=instr_fetch(eip+1,DATA_BYTE);
	print_asm("je" str(SUFFIX) " %x",eip+1+imm+DATA_BYTE);
	if(cpu.ZF==1) cpu.eip+=imm;
	return DATA_BYTE+1;
}

#undef instr

#include "cpu/exec/template-end.h"
