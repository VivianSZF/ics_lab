#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute(){
	DATA_TYPE result;
	if(DATA_BYTE==1){
		result=cpu.gpr[0]._8[0]-cpu.gpr[7]._8[0];
		cpu.CF=(cpu.gpr[0]._8[0]<cpu.gpr[7]._8[0]);
		cpu.OF=((MSB(cpu.gpr[0]._8[0])!=MSB(cpu.gpr[7]._8[0]))&&(MSB(result)!=MSB(cpu.gpr[0]._8[0])));
	}
	else if(DATA_BYTE==2){
		result=cpu.gpr[0]._16-cpu.gpr[7]._16;
		cpu.CF=(cpu.gpr[0]._16<cpu.gpr[7]._16);
		cpu.OF=((MSB(cpu.gpr[0]._16)!=MSB(cpu.gpr[7]._16))&&(MSB(result)!=MSB(cpu.gpr[0]._16)));
	}
	else{
		result=cpu.eax-cpu.edi;
		cpu.CF=(cpu.eax<cpu.edi);
		cpu.OF=((MSB(cpu.eax)!=MSB(cpu.edi))&&(MSB(result)!=MSB(cpu.eax)));
	}
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
