#include "cpu/exec/helper.h"

extern void device_update();

make_helper(hlt){
	while(!cpu.INTR){
		device_update();
	}
	print_asm("hlt");
	return 1;
}

