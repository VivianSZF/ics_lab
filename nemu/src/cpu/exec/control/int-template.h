#include "cpu/exec/template-start.h"

#define instr int

void raise_intr(uint8_t);

static void do_execute(){
	print_asm_template1();
	cpu.eip+=getinstr_len();
	raise_intr(op_src->val);
}

make_instr_helper(i);

#include "cpu/exec/template-end.h"
