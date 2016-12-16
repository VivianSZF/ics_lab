#include "cpu/exec/helper.h"
#include "../../lib-common/x86-inc/mmu.h"
#include <setjmp.h>
//extern jmp_buf jbuf;

make_helper(iret){
	cpu.eip=swaddr_read(cpu.esp,4,S_SS);
	cpu.esp+=4;
	cpu.CS=(uint16_t)swaddr_read(cpu.esp,4,S_SS);
	cpu.esp+=4;

	uint8_t t[8];
	int i;
	for(i=0;i<8;i++)
		t[i]=lnaddr_read(cpu.GDTR.base+cpu.sreg[S_CS].INDEX*8+i,1);
	SegDesc *segd=(SegDesc *)t;
	cpu.segcache[S_CS].base=segd->base_15_0+(segd->base_23_16<<16)+(segd->base_31_24<<24);
	cpu.segcache[S_CS].valid=true;
	
	cpu.eflags_val=swaddr_read(cpu.esp,4,S_SS);
	cpu.esp+=4;
//	longjmp(jbuf,1);
	print_asm("iret");
	return 0;
}
