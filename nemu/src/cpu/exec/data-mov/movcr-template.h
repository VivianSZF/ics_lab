#include "cpu/exec/template-start.h"

#define instr movcr

make_helper(concat(movcr_cr2r_,SUFFIX)){
	uint32_t num=instr_fetch(eip+1,1);
	uint32_t reg=num&0x7;
	uint32_t choose=(instr_fetch(eip+1,1)>>3)&0x7;
	if(choose==0)
		reg_l(reg)=cpu.cr0.val;
	else if(choose==3)
		reg_l(reg)=cpu.cr3.val;
	print_asm("mov cr%d %%%s",choose,regsl[reg]);
	return 2;
}

make_helper(concat(movcr_r2cr_,SUFFIX)){
	uint32_t num=instr_fetch(eip+1,1);
	uint32_t reg=num&0x7;
	uint32_t choose=(instr_fetch(eip+1,1)>>3)&0x7;
	if(choose==0)
		cpu.cr0.val=reg_l(reg);
	else if(choose==3)
		cpu.cr3.val=reg_l(reg);
	print_asm("mov %%%s cr%d",regsl[reg],choose);
	return 2;
} 

#include "cpu/exec/template-end.h"
