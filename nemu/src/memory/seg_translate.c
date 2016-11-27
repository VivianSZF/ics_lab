#include "common.h"
#include "cpu/reg.h"
#include "memory/memory.h"

lnaddr_t seg_translate(swaddr_t addr,size_t len, uint8_t sreg){
	if(cpu.cr0.protect_enable==0)
		return addr;
	lnaddr_t lnaddr;
	uint32_t base;
	if (cpu.segcache[sreg].valid){
		base=cpu.segcache[sreg].base;
	}
	else{
		base=cpu.creg[sreg].base_15_0+(cpu.creg[sreg].base_23_16<<16)+(cpu.creg[sreg].base_31_24<<24);
		cpu.segcache[sreg].base=base;
		cpu.segcache[sreg].valid=true;
	}
	lnaddr=addr+base;
	return lnaddr;
}

