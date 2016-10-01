#include "cpu/exec/template-start.h"

#define instr cltd

static void do_execute(){
	DATA_TYPE t=(DATA_BYTE<<3)-1;
	if(cpu.eax>>t==1)
		cpu.edx=(1<<(t+1))-1;
	else
		cpu.edx=0;
	print_asm("cltd");
}

make_instr_helper(n);

#include "cpu/exec/template-end.h"
