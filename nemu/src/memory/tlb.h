#include "common.h"

#define TLB_NUM 64

typedef struct{
	uint32_t data;
	uint32_t tag;
	bool validbit;
}TLB;

TLB tlb[TLB_NUM];
