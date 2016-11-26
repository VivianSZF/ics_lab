#include "cpu/exec/template-start.h"

#define instr movcr

make_helper(concat(movcr_cr2r,SUFFIX)){
	uint32_t num=instr_fetch(eip+1,1);
	uint32_t reg=num&0x7;
	reg_l(reg)=cpu.cr0.val;
	print_asm("mov cr0 %%%s",regsl[reg]);
	return 2;
}

make_helper(concat(movcr_r2cr,SUFFIX)){
	uint32_t num=instr_fetch(eip+1,1);
	uint32_t reg=num&0x7;
	cpu.cr0.val=reg_l(reg);
	print_asm("mov %%%s cr0",regsl[reg]);
	return 2;
}

#include "cpu/exec/template-end.h"
