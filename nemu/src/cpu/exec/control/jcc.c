#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE

make_helper_v(je_i)
make_helper_v(jbe_i)
make_helper_v(jne_i)
make_helper_v(jle_i)
make_helper_v(jg_i)
make_helper_v(jl_i)
