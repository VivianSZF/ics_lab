#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute(){
	DATA_TYPE src=REG(R_EAX);
	//DATA_TYPE src=cpu.eax;
	DATA_TYPE dest=MEM_R(REG(R_EDI));
	//DATA_TYPE dest=swaddr_read(cpu.edi,DATA_BYTE);
	DATA_TYPE result=src-dest;
	cpu.CF=(src<dest);
	cpu.OF=((MSB(src)!=MSB(dest))&&(MSB(result)!=MSB(src)));
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
