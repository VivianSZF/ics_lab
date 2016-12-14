#include "cpu/exec/template-start.h"

#define instr out

void pio_write(ioaddr_t,size_t,uint32_t);

make_helper(concat(out_i_,SUFFIX)){
	uint16_t p=instr_fetch(eip+1,1);
	pio_write(p,DATA_BYTE,(DATA_TYPE)REG(R_EAX));
	print_asm("out");
	return 2;
}

make_helper(concat(out_r_,SUFFIX)){
	uint16_t p=reg_w(R_DX);
	pio_write(p,DATA_BYTE,(DATA_TYPE)REG(R_EAX));
	print_asm("out");
	return 1;
}

#include "cpu/exec/template-end.h"
