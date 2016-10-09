#include "cpu/exec/template-start.h"

#define instr ja
static void do_execute(){
	DATA_TYPE_S dis=op_src->val;
	if(cpu.CF==0&&cpu.ZF==0){
		cpu.eip+=dis;
		print_asm("ja %x",cpu.eip+len+1);
	}
	else
	{
		print_asm("ja %x",cpu.eip+op_src->val+len+1);
	}
}
make_instr_helper(i)
#undef instr

#define instr je
static void do_execute(){
	//printf("len %d\n",len);
	//printf("da %d\n",DATA_BYTE);
	//printf("eip %x\n",cpu.eip);
	//printf("op_src %x\n",op_src->val);
	DATA_TYPE_S dis=op_src->val;
 	if(cpu.ZF==1){ 
		cpu.eip+=dis;
		print_asm("je %x",cpu.eip+1+len);
   	}
	else
  	{
		print_asm("je %x",cpu.eip+op_src->val+1+len);
     }
} 
make_instr_helper(i)
#undef instr

#define instr jbe
static void do_execute(){
	//printf("%d\n",len);
	//printf("%d\n",DATA_BYTE);
	//printf("%d\n",op_src->val);
	//printf("%x\n",cpu.eip);
	DATA_TYPE_S dis=op_src->val;
 	if(cpu.CF==1||cpu.ZF==1){
		cpu.eip+=dis;
		print_asm("jbe %x",cpu.eip+len+1);
	}
	else
 	{ 
		print_asm("jbe %x",cpu.eip+dis+len+1);
	}
}
make_instr_helper(i)
#undef instr

#define instr jne
static void do_execute(){
	DATA_TYPE_S dis=op_src->val;
	if(cpu.ZF==0){
		cpu.eip+=dis;
		print_asm("jne %x",cpu.eip+len+1);
	}
	else
	{
		print_asm("jne %x",cpu.eip+dis+len+1);
	}
} 
make_instr_helper(i)
#undef instr

#define instr jns
static void do_execute(){
	DATA_TYPE_S dis=op_src->val;
	if(cpu.SF==0){
		cpu.eip+=dis;
		print_asm("jns %x",cpu.eip+len+1);
	}
	else
	{
		print_asm("jns %x",cpu.eip+dis+len+1);
	}
}
make_instr_helper(i)
#undef instr

#define instr jle
static void do_execute(){
	DATA_TYPE_S dis=op_src->val;
	if(cpu.ZF==1||cpu.SF!=cpu.OF){
		cpu.eip+=dis;
		print_asm("jle %x",cpu.eip+len+1);
 	}
	else
	{
		print_asm("jle %x",cpu.eip+dis+len+1);
 	}
} 
make_instr_helper(i)
#undef instr

#define instr jg
static void do_execute(){
	DATA_TYPE_S dis=op_src->val;
	if(cpu.ZF==0&&cpu.SF==cpu.OF){
		cpu.eip+=dis;
		print_asm("jg %x",cpu.eip+len+1);
	}
	else
	{
		print_asm("jg %x",cpu.eip+dis+len+1);
	}
}
make_instr_helper(i)
#undef instr

#define instr jge
static void do_execute(){
	DATA_TYPE_S dis=op_src->val;
	if(cpu.SF==cpu.OF){
		cpu.eip+=dis;
		print_asm("jge %x",cpu.eip+len+1);
	}
	else
	{
		print_asm("jge %x",cpu.eip+dis+len+1);
	}
}
make_instr_helper(i)
#undef instr

#define instr jl
static void do_execute(){
	DATA_TYPE_S dis=op_src->val;
	if(cpu.SF!=cpu.OF){
		cpu.eip+=dis;
		print_asm("jl %x",cpu.eip+len+1);
 	}
	else
 	{
		print_asm("jl %x",cpu.eip+dis+len+1);
	}
 }
make_instr_helper(i)
#undef instr

#define instr js
static void do_execute(){
	DATA_TYPE_S dis=op_src->val;
	if(cpu.SF==1){
		cpu.eip+=dis;
		print_asm("js %x",cpu.eip+len+1);
	}
	else
	{
		print_asm("js %x",cpu.eip+dis+len+1);
	}
}
make_instr_helper(i)
#undef instr

#include "cpu/exec/template-end.h"
