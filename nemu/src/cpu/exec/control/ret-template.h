#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute(){
 	/*if(DATA_BYTE==2){
		cpu.eip=swaddr_read(cpu.esp,2,S_SS);
		cpu.esp+=2+op_src->val;
		cpu.eip=cpu.eip&0xffff;
	}*/
		cpu.eip=swaddr_read(cpu.esp,4,S_SS);
		cpu.esp+=4+op_src->val;
	if(op_src->val==0)
		print_asm("ret");
 	else{
		cpu.eip-=2;
		print_asm_template1();
	}
}


make_instr_helper(n)
make_instr_helper(si)

#include "cpu/exec/template-end.h"
