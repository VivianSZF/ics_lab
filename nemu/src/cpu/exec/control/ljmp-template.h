#include "cpu/exec/template-start.h"

#define instr ljmp

make_helper(concat(ljmp_,SUFFIX)){
	uint8_t sreg=instr_fetch(cpu.eip+1+DATA_BYTE,2);
	swaddr_t addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	cpu.eip=addr-7;
	cpu.CS=sreg;
	cpu.segcache[1].valid=false;
	printf("!%x\n",DATA_BYTE);
	print_asm("ljmp $0x%x $0x%x",sreg,addr);
	return DATA_BYTE+3;
}
#include "cpu/exec/template-end.h"
