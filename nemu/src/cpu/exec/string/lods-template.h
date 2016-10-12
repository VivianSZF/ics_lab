#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute(){
	if(DATA_BYTE==1){
		cpu.gpr[0]._8[0]=swaddr_read(cpu.esi,1);
	}
	else if(DATA_BYTE==2){
		cpu.gpr[0]._16=swaddr_read(cpu.esi,2);
	}
	else{
		cpu.eax=swaddr_read(cpu.esi,4);
	}
	
	//REG(R_EAX)=MEM_R(REG(R_ESI));
	if(cpu.DF==0)
		cpu.esi+=DATA_BYTE;
	else
		cpu.esi-=DATA_BYTE;
	print_asm_template2();
}

make_instr_helper(n);

#include "cpu/exec/template-end.h"

