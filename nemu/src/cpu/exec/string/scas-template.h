#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute(){
	op_src->type=op_dest->type=OP_TYPE_REG;
	op_src->reg=R_EAX;
	op_dest->reg=R_EDI;
	op_src->val=cpu.eax;
	op_dest->val=swaddr_read(cpu.edi,DATA_BYTE);
	snprintf(op_src->str,11,"%%es:(%%edi)");
	snprintf(op_dest->str,5,"%%eax");
	//DATA_TYPE src=REG(R_EAX);
	//DATA_TYPE src=cpu.eax;
	//DATA_TYPE dest=MEM_R(REG(R_EDI));
	//DATA_TYPE dest=swaddr_read(cpu.edi,DATA_BYTE);
	DATA_TYPE result=op_src->val-op_dest->val;
	cpu.CF=(op_src->val<op_dest->val);
	cpu.OF=((MSB(op_src->val)!=MSB(op_dest->val))&&(MSB(result)!=MSB(op_src->val)));
	if(cpu.DF==0)
		cpu.edi+=DATA_BYTE;
	else
		cpu.edi-=DATA_BYTE;
	DATA_TYPE pf=result;
	pf=pf^(pf>>4);
	pf=pf^(pf>>2);
	pf=pf^(pf>>1);
	pf=pf&1;
	cpu.PF=!pf;
	cpu.ZF=(result==0);
	cpu.SF=MSB(result);
	print_asm_template2();
}

make_instr_helper(n);

#include "cpu/exec/template-end.h"
