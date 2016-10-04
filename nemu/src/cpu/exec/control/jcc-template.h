#include "cpu/exec/template-start.h"

#define instr je
static void do_execute(){
	if(cpu.ZF==1){ 
		cpu.eip+=op_src->val;
		print_asm("je"str(SUFFIX) " %x",cpu.eip+1+len);
  	}
	else
 	{
		print_asm("je"str(SUFFIX) " %x",cpu.eip+op_src->val+1+len);
 	}
} 
make_instr_helper(i)
#undef instr

#define instr jbe
static void do_execute(){
	printf("%d\n",len);
	printf("%d\n",op_src->val);
	printf("%x\n",cpu.eip);
	DATA_TYPE_S dis=op_src->val;
	if(cpu.CF==1||cpu.ZF==1){
		cpu.eip+=dis;
		print_asm("jbe %x",cpu.eip+DATA_BYTE+1);
	}
	else
	{
		print_asm("jbe %x",cpu.eip+dis+DATA_BYTE+1);
	}
}
make_instr_helper(i)
#undef instr

#include "cpu/exec/template-end.h"
