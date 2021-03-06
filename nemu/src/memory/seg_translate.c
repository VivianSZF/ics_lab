#include "common.h"
#include "cpu/reg.h"
#include "memory/memory.h"
#include "../../lib-common/x86-inc/mmu.h"

lnaddr_t seg_translate(swaddr_t addr, uint8_t sreg){
	if(cpu.cr0.protect_enable==0)
		return addr;
	lnaddr_t lnaddr;
	uint32_t base;
	if (cpu.segcache[sreg].valid){
		base=cpu.segcache[sreg].base;
	}
	else{
		uint8_t p[8];
		int i;
		for(i=0;i<8;i++)
			p[i]=lnaddr_read(cpu.GDTR.base+cpu.sreg[sreg].INDEX*8+i,1);
		SegDesc *sd=(SegDesc*)p; 
		base=sd->base_15_0+(sd->base_23_16<<16)+(sd->base_31_24<<24);
		cpu.segcache[sreg].base=base;
		cpu.segcache[sreg].valid=true;
	//	printf("%x %x %x %x\n",cpu.sreg[sreg].INDEX,base,sreg,cpu.GDTR.base);
	}
//	printf("%x  %x\n",cpu.sreg[sreg].INDEX,base);
	lnaddr=addr+base;
	return lnaddr;
}

void synthesreg(uint8_t sreg) {
	uint8_t p[8]; 
	int i;
	for(i=0;i<8;i++) 
		p[i]=lnaddr_read(cpu.GDTR.base+cpu.sreg[sreg].INDEX*8+i,1);
	SegDesc *sd= (SegDesc*)p;
	uint32_t base;
	base=sd->base_15_0+(sd->base_31_24 << 24)+(sd->base_23_16<<16);
	cpu.segcache[sreg].base=base;
	cpu.segcache[sreg].valid=true;
}



