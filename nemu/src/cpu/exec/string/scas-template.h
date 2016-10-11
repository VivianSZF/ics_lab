#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute(){
	DATA_TYPE_S result=REG(R_EAX)-MEM_R(REG(R_EDI));
	cpu.CF=(REG(R_EAX)<MEM_R(REG(R_EDI)));
	cpu.OF=((MSB(REG(R_EAX))!=MSB(MEM_R(REG(R_EDI))))&&(MSB(result)!=MSB(REG(R_EAX))));
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
