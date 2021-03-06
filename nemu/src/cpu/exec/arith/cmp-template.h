#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
//	printf("%x\n",cpu.eip);
	DATA_TYPE_S src=op_src->val;
	DATA_TYPE_S dest=op_dest->val;
	DATA_TYPE_S result=dest-src;
	DATA_TYPE pf=result;
	pf=pf^(pf>>4);
	pf=pf^(pf>>2);
	pf=pf^(pf>>1);
	pf=pf&1;
	cpu.PF=!pf;
	cpu.ZF=(result==0);
	cpu.SF=MSB(result);
	cpu.CF=(op_dest->val<op_src->val);
	if((MSB(op_dest->val)!=MSB(op_src->val))&&(MSB(result)!=MSB(op_dest->val)))
		cpu.OF=1;
	else
		cpu.OF=0;
	print_asm_template2();
}

make_instr_helper(i2a);
make_instr_helper(i2rm);
#if DATA_BYTE==2||DATA_BYTE==4
make_instr_helper(si2rm);
#endif
make_instr_helper(r2rm);
make_instr_helper(rm2r);

#include "cpu/exec/template-end.h"
