#include "tlb.h"

void init_TLB(){
	int i;
	for(i=0;i<TLB_NUM;i++)
		tlb[i].validbit=false;
}

