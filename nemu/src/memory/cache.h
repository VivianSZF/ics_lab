#include "common.h"
#include "burst.h"
#include "misc.h"
#include <time.h>
#include <stdlib.h>

#define L1_OFFSET 6
#define L1_INDEX 7
#define L1_TAG (32-L1_OFFSET-L1_INDEX)

#define L2_OFFSET 6
#define L2_INDEX 12
#define L2_TAG (32-L2_OFFSET-L2_INDEX)

typedef union{
	struct{
		uint32_t offset1 :L1_OFFSET;
		uint32_t index1  :L1_INDEX;
		uint32_t tag1    :L1_TAG;
	};
	struct{
		uint32_t offset2 :L2_OFFSET;
		uint32_t index2  :L2_INDEX;
		uint32_t tag2    :L2_TAG;
	};
	uint32_t addr;
}cache_addr;

