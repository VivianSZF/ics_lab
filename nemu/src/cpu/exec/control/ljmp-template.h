#include "cpu/exec/template-start.h"

#define instr ljmp

make_helper(concat(ljmp_,SUFFIX)){
	cpu.rseg[S_CS].val=instr_fetch(cpu.eip+1+DATA_BYTE,2);
	int offset=cpu.rseg[S_CS].INDEX*8;
	cpu.creg[S_CS].val=lnaddr_read(cpu.GDTR.base+offset,4)+(((long long)(lnaddr_read(cpu.GDTR.base+offset+4,4)))<<32);
	swaddr_t addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	cpu.eip=addr-7;
	cpu.segcache[1].valid=false;
	print_asm("ljmp $0x%x $0x%x",cpu.rseg[S_CS].val,addr);
	return DATA_BYTE+3;
}
#include "cpu/exec/template-end.h"
