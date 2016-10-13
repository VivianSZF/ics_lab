#include "cpu/exec/template-start.h"

#define instr stos

static void do_execute(){
	/*if(DATA_BYTE==1){
		swaddr_write(cpu.edi,DATA_BYTE,REG(R_AL));
	}
	else if(DATA_BYTE==2){
		swaddr_write(cpu.edi,DATA_BYTE,REG(R_AX));
	}
	else{
		swaddr_write(cpu.edi,DATA_BYTE,REG(R_EAX));
	}*/
	swaddr_write(cpu.edi,DATA_BYTE,(DATA_TYPE)cpu.eax);
	if(cpu.DF==0)
		cpu.edi+=DATA_BYTE;
	else
		cpu.edi-=DATA_BYTE;
	print_asm_template2();
}

make_instr_helper(n);

#include "cpu/exec/template-end.h"
