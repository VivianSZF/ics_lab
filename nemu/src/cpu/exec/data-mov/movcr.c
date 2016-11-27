#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "movcr-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "movcr-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movcr-template.h"
#undef DATA_BYTE

//make_helper_v(movcr_r2cr);
//make_helper_v(movcr_cr2r);
/*make_helper(movcr_cr2r){
	uint32_t num=instr_fetch(eip+1,1);
	uint32_t reg=num&0x7;
	reg_l(reg)=cpu.cr0.val;
	print_asm("mov cr0 %%%s",regsl[reg]);
	return 2;
}
make_helper(movcr_r2cr){
	uint32_t num=instr_fetch(eip+1,1);
	uint32_t reg=num&0x7;
	cpu.cr0.val=reg_l(reg);
	print_asm("mov %%%s cr0",regsl[reg]);
	return 2;
}*/
