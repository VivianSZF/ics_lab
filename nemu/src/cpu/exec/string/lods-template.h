#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute(){
	if(DATA_BYTE==1){
		REG(R_AL)=swaddr_read(cpu.esi,1);
	}
	else if(DATA_BYTE==2){
		REG(R_AX)=swaddr_read(cpu.esi,2);
	}
	else{
		REG(R_EAX)=swaddr_read(cpu.esi,4);
	}
	if(cpu.DF==0)
		cpu.esi+=DATA_BYTE;
	else
		cpu.esi-=DATA_BYTE;
	print_asm_template2();
}

make_instr_helper(n);

#include "cpu/exec/template-end.h"

