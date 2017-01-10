#include "cache.h"
#include<stdio.h>

#define WAY_SIZE 8
#define BLOCK_SIZE (1<<L1_OFFSET)
#define INDEX_SIZE (1<<L1_INDEX)
#define MASK (BLOCK_SIZE-1)
#define HW_MEM_SIZE (1<<27)

typedef struct {
	uint8_t data[WAY_SIZE][BLOCK_SIZE];
	uint32_t tag[WAY_SIZE];
	bool validbit[WAY_SIZE];
}L1_cache;

L1_cache l1cache[INDEX_SIZE];

uint32_t l2read(hwaddr_t, size_t);

void init_l1cache(){
	memset(l1cache,0,sizeof(l1cache));
}

static int hitornot(uint32_t index,uint32_t tag){
	uint32_t i;
	for(i=0;i<WAY_SIZE;i++){
		if(l1cache[index].validbit[i]&&(l1cache[index].tag[i]==tag))
				break;
  	}
	return i;
 }

static int fullornot(uint32_t index){
	uint32_t i;
	for(i=0;i<WAY_SIZE;i++){
		if(!l1cache[index].validbit[i])
			break;
 	}
	return i;
}

static void l1set_read(hwaddr_t addr, void*data){
	Assert(addr<HW_MEM_SIZE,"physical address %x is outside of the physical memory!",addr);
	cache_addr ad;
	ad.addr=addr&~BURST_MASK;
	uint32_t of=ad.offset1;
	uint32_t in=ad.index1;
	uint32_t ta=ad.tag1;
	uint32_t ans=hitornot(in,ta);
	
/*
	uint32_t i;
	for(i=0;i<WAY_SIZE;i++){
		if(l1cache[index].validbit[i]&&(l1cache[index].tag[i]==tag))
				break;
  	}

	bool full=true;
	bool find=false;
	uint32_t row, k;
	for (row=0; row<WAY_SIZE; row++) {
		if (l1cache[in].validbit[row]) {
			if (l1cache[in].tag[row]==ta) {
				memcpy(data, l1cache[in].data[row]+of, BURST_LEN);
				find=true;
				break;
			}
			
		}
		else {
				full=false;
				k=row;
			}
	}
	if (find==false) {
		if (full) {
			srand(time(0));
			k=rand()%WAY_SIZE;
		}

		l1cache[in].validbit[k]=true;
		l1cache[in].tag[k]=ta;
		uint32_t i;
		for (i=0; i<BLOCK_SIZE; i++) {
			l1cache[in].data[k][i]=l2read((addr & (~MASK))+i, 1);
		}
		memcpy(data, l1cache[in].data[k]+of, BURST_LEN);
	}
*/

	if(ans<8){
		memcpy(data,l1cache[in].data[ans]+of,BURST_LEN);
 	}
	else{
		ans=fullornot(in);
 		if(ans>=8){
			srand(time(0));
			ans=rand()%WAY_SIZE;
		}
		l1cache[in].validbit[ans]=true;
		l1cache[in].tag[ans]=ta;
		int i;
		for(i=0;i<BLOCK_SIZE;i++)
			l1cache[in].data[ans][i]=l2read((addr&(~MASK))+i,1);
		memcpy(data,l1cache[in].data[ans]+of,BURST_LEN);
 		}
} 
uint32_t l1read(hwaddr_t addr,size_t len){
	uint32_t offset=addr&BURST_MASK;
	uint8_t temp[2*BURST_LEN];
	l1set_read(addr,temp);
 	if(offset+len>BURST_LEN){
		l1set_read(addr+BURST_LEN,temp+BURST_LEN);
	}
	return unalign_rw(temp+offset,4);
}

static void l1set_write(hwaddr_t addr,void *data,uint8_t *mask){
	Assert(addr<HW_MEM_SIZE,"phsical address %x is outside of the physical memory!",addr);
	cache_addr ad;
	ad.addr=addr&~BURST_MASK;
	uint32_t of=ad.offset1;
	uint32_t in=ad.index1;
	uint32_t ta=ad.tag1;
	uint32_t ans=hitornot(in,ta);
 	if(ans<8){
		memcpy_with_mask(l1cache[in].data[ans]+of,data,BURST_LEN,mask);
	}
	/*
	uint32_t row;
	for (row=0; row<WAY_SIZE; row++) {
		if(l1cache[in].validbit[row] && l1cache[in].tag[row]==ta) {
			memcpy_with_mask(l1cache[in].data[row] + of, data, BURST_LEN, mask);
			break;
		}
	}
	*/
	
}
void l1write(hwaddr_t addr,size_t len,uint32_t data){
	uint32_t offset=addr&BURST_MASK;
	uint8_t temp[2*BURST_LEN];
	uint8_t mask[2*BURST_LEN];
	memset(mask,0,2*BURST_LEN);
	*(uint32_t*)(temp+offset)=data;
	memset(mask+offset,1,len);
	l1set_write(addr,temp,mask);
	if(offset+len>BURST_LEN){
		l1set_write(addr+BURST_LEN,temp+BURST_LEN,mask+BURST_LEN);
 	}
}
