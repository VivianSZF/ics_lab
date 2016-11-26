#include "common.h"
#include "cpu/reg.h"
#include "memory/memory.h"

lnaddr_t seg_translate(swaddr_t addr,size_t len, uint8_t sreg){
	if(cpu.cr0.protect_enable==0)
		return addr;
	else{
		lnaddr_t lnaddr;
		uint32_t base=cpu.creg[sreg].base_15_0+(cpu.creg[sreg].base_23_16<<16)+(cpu.creg[sreg].base_31_24<<24);
		lnaddr=addr+base;
		return lnaddr;
	}
}

