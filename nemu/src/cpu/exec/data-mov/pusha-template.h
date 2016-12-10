#include "cpu/exec/template-start.h"

#define instr pusha

static void do_execute(){
	uint32_t temp=cpu.esp;
	int i;
	for(i=0;i<8;i++){
		cpu.esp-=DATA_BYTE;
		if(i==4){
			swaddr_write(cpu.esp,DATA_BYTE,temp,S_SS);
		}
		else{
			swaddr_write(cpu.esp,DATA_BYTE,cpu.gpr[i]._32,S_SS);
		}
	}
	print_asm("pusha");
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
