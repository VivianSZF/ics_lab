#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute(){

	if(ops_decoded.opcode==0x01bf){
		int16_t num=op_src->val;
		REG(op_dest->reg)=num;
	}	
	else
	{	
		int8_t num=op_src->val;
		REG(op_dest->reg)=num;
	}
	print_asm_template2();
}

make_instr_helper(rmb2r);
make_instr_helper(rmw2r);

#include "cpu/exec/template-end.h"
