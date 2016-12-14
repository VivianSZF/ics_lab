#include "cpu/exec/template-start.h"

#define instr int

void raise_intr(uint8_t, uint32_t);

static void do_execute(){
	print_asm_template1();
	uint32_t len=getinstr_len();
	raise_intr(op_src->val,len);
}

make_instr_helper(i);

#include "cpu/exec/template-end.h"
