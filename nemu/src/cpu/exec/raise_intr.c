#include <setjmp.h>
#include "common.h"
#include "../../lib-common/x86-inc/mmu.h"
#include "cpu/reg.h"
#include "cpu/exec/helper.h"

extern jmp_buf jbuf;
void synthesreg(uint8_t);
void raise_intr(uint8_t NO){
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eflags_val,S_SS);
//	printf("fine1\n");
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.CS,S_SS);
//	printf("fine2\n");
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip,S_SS);
//	printf("fine3\n");
	uint8_t t[8];
	int i;
	for(i=0;i<8;i++){
		t[i]=lnaddr_read(cpu.IDTR.base+NO*8+i,1);
//		printf("fine\n");
 	}
	GateDesc *gate=(GateDesc *)t;
	cpu.CS=gate->segment;
	
	synthesreg(S_CS);

	cpu.eip=gate->offset_15_0+(gate->offset_31_16<<16);

	longjmp(jbuf,1);
}
