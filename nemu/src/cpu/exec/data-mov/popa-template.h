#include "cpu/exec/template-start.h"

#define instr popa

static void do_execute(){
	int i;
	for(i=7;i>=0;i--){
		if(i!=4){
			cpu.gpr[i]._32=swaddr_read(cpu.esp,DATA_BYTE,S_SS);
		}
		cpu.esp+=DATA_BYTE;
	}
	print_asm("popa");
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
