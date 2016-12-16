#include "cpu/exec/template-start.h"

#define instr in

uint32_t pio_read(ioaddr_t,size_t);
/*
make_helper(concat(in_i_,SUFFIX)){
	uint16_t p=instr_fetch(eip+1,1);
	REG(R_EAX)=(DATA_TYPE)pio_read(p,DATA_BYTE);
	print_asm("in");
	return 2;
}

make_helper(concat(in_r_,SUFFIX)){
	uint16_t p=reg_w(R_DX);
	REG(R_EAX)=(DATA_TYPE)pio_read(p,DATA_BYTE);
	print_asm("in");
	return 1;
}
*/
make_helper(concat(in_,SUFFIX)){
		concat(reg_,SUFFIX)(R_EAX)=pio_read(reg_w(R_DX),DATA_BYTE);
		print_asm("in");
		return 1;
		}
#include "cpu/exec/template-end.h"
