#include <setjmp.h>
#include "common.h"
#include "../../lib-common/x86-inc/mmu.h"
#include "cpu/exec/helper.h"

extern jmp_buf jbuf;

void raise_intr(uint8_t NO,uint32_t len){
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eflags_val,S_SS);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.CS,S_SS);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip+len,S_SS);
	
	uint8_t t[8];
	int i;
	for(i=0;i<8;i++)
		t[i]=lnaddr_read(cpu.IDTR.base+NO*8+i,1);
	GateDesc *gate=(GateDesc *)t;
	cpu.CS=gate->segment;
	
	uint8_t t1[8];
	for(i=0;i<8;i++)
		t1[i]=lnaddr_read(cpu.GDTR.base+cpu.sreg[S_CS].INDEX*8+i,1);
	SegDesc *segd=(SegDesc *)t1;
	cpu.segcache[S_CS].base=segd->base_15_0+(segd->base_23_16<<16)+(segd->base_31_24<<24);
	cpu.segcache[S_CS].valid=true;

	cpu.eip=gate->offset_15_0+(gate->offset_31_16<<16);

	longjmp(jbuf,1);
}
