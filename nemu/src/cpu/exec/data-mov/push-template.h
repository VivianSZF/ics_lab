#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
	if(DATA_BYTE==2){
		cpu.esp-=2;
		swaddr_write(cpu.esp,2,op_src->val,S_SS);
	}
	else if(DATA_BYTE==4){
		cpu.esp-=4;
		swaddr_write(cpu.esp,4,op_src->val,S_SS);
	}
	else if(DATA_BYTE==1){
		int val=(DATA_TYPE_S)op_src->val;
		cpu.esp-=4;
		swaddr_write(cpu.esp,4,val,S_SS);
	}
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
