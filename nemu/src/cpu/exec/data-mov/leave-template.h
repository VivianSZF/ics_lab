#include "cpu/exec/template-start.h"

#define instr leave

static void do_execute(){
	cpu.esp=cpu.ebp;
	cpu.ebp=swaddr_read(cpu.esp,4,S_SS);
	cpu.esp+=4;
	print_asm("leave");
}

make_instr_helper(n);

#include"cpu/exec/template-end.h"
