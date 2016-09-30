#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute(){
	DATA_TYPE num=op_src->val;
}

#include "cpu/exec/template-end.h"
