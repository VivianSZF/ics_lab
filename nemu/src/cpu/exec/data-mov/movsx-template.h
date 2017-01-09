#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute(){
	DATA_TYPE num;
	if(ops_decoded.opcode==0x01bf)
		num=(((DATA_TYPE_S)op_src->val)<<16)>>16;
	else
	{	
		uint32_t t=(DATA_BYTE<<3)-8;
		num=(((DATA_TYPE_S)op_src->val)<<t)>>t;
	}
	OPERAND_W(op_dest,num);
	print_asm_template2();
}

make_instr_helper(rmb2r);
make_instr_helper(rmw2r);

#include "cpu/exec/template-end.h"
