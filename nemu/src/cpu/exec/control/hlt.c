#include "cpu/exec/helper.h"

make_helper(hlt){
	while(!cpu.INTR);
	print_asm("hlt");
	return 1;
}

