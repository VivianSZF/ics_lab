#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute(){
	DATA_TYPE_S src=op_src->val;
	DATA_TYPE result=op_dest->val-src;
	DATA_TYPE pf=result;
	pf=pf^(pf>>4);
	pf=pf^(pf>>2);
	pf=pf^(pf>>1);
	pf=pf&1;
	cpu.PF=!pf;
	cpu.ZF=(result==0);
	cpu.SF=MSB(result);
	cpu.CF=(result<op_dest->val&&src>0);
	if((MSB(op_dest->val)!=MSB(src))&&(MSB(result)==MSB(src)))
		cpu.OF=1;
	else
		cpu.OF=0;
	OPERAND_W(op_dest,result);
	print_asm_template2();
}

#if DATA_BYTE==2||DATA_BYTE==4
make_instr_helper(si2rm)
#endif
make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
