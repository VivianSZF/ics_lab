#include "cpu/exec/template-start.h"

#define instr test

static void do_execute(){
	DATA_TYPE result=op_dest->val & op_src->val;
	DATA_TYPE pf=result;
	pf=pf^(pf>>4);
	pf=pf^(pf>>2);
	pf=pf^(pf>>1);
	pf=pf&1;
	cpu.PF=!pf;
	cpu.ZF=(result==0);
	cpu.SF=MSB(result);
	cpu.CF=0;
	cpu.OF=0;
	print_asm_template2();
}

make_instr_helper(i2a);
make_instr_helper(i2rm);
make_instr_helper(r2rm);

#include "cpu/exec/template-end.h"

