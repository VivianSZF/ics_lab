#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute(){
	swaddr_write(cpu.edi,DATA_BYTE,swaddr_read(cpu.esi,DATA_BYTE,S_DS),S_ES);
	if(cpu.DF==0){
		cpu.edi+=DATA_BYTE;
		cpu.esi+=DATA_BYTE;
	}
	else
	{
		cpu.edi-=DATA_BYTE;
		cpu.esi-=DATA_BYTE;
	}
	print_asm_template2();
}

make_instr_helper(n);

#include "cpu/exec/template-end.h"
