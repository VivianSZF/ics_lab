#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src,result);
	DATA_TYPE pf=result;
	pf=pf^(pf>>4);
	pf=pf^(pf>>2);
	pf=pf^(pf>>1);
	pf=pf&1;
	cpu.PF=!pf;
	cpu.ZF=(result==0);
	cpu.SF=MSB(result);
	if((MSB(result)==MSB(op_src->val))&&(MSB(op_src->val)!=MSB(result)))
		cpu.OF=1;
	else
		cpu.OF=0;

	/* TODO: Update EFLAGS. */
	//panic("please implement me");
	
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
