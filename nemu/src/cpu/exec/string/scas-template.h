#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute(){
	//op_src->type=op_dest->type=OP_TYPE_REG;
	//op_src->reg=R_EAX;
	//op_dest->reg=R_EDI;
	uint32_t rega;
	if(DATA_BYTE==1){
		rega=R_AL;
	}
	else if(DATA_BYTE==2){
		rega=R_AX;
	}
	else{
		rega=R_EAX;
	}
	//op_src->val=REG(R_EAX);
	//op_dest->val=swaddr_read(cpu.edi,DATA_BYTE);
	//snprintf(op_src->str,11,"%%es:(%%edi)");
	//snprintf(op_dest->str,5,"%%eax");
	//DATA_TYPE src=REG(R_EAX);
	//DATA_TYPE src=cpu.eax;
	DATA_TYPE src=REG(rega);
	DATA_TYPE dest=swaddr_read(reg_l(R_EDI),DATA_BYTE);
	//DATA_TYPE dest=swaddr_read(cpu.edi,DATA_BYTE);
	DATA_TYPE_S result=src-dest;
	//printf("src: %x  dest:%x  result:%d\n",src,dest,result);
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
