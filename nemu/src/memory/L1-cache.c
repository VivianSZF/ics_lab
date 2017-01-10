#include "cache.h"

#define WAY_SIZE 8
#define BLOCK_SIZE (1<<L1_OFFSET)
#define INDEX_SIZE (1<<L1_INDEX)
#define MASK (BLOCK_SIZE-1)
#define HW_MEM_SIZE (1<<27)

typedef struct{ 
	uint8_t d[BLOCK_SIZE];
	uint32_t tag;
	bool validbit;
}LINE;
typedef struct{
	LINE line[WAY_SIZE];
}SET;
typedef struct{
	SET set[INDEX_SIZE];
}L1_cache;

L1_cache l1cache;

uint32_t l2read(hwaddr_t, size_t);

void init_l1cache(){
	int i,j;
	for(i=0;i<INDEX_SIZE;i++)
		for(j=0;j<WAY_SIZE;j++)
			l1cache.set[i].line[j].validbit=false;
}

static int hitornot(uint32_t index,uint32_t tag){
	uint32_t i;
	for(i=0;i<WAY_SIZE;i++){
		if(l1cache.set[index].line[i].validbit&&l1cache.set[index].line[i].tag==tag)
				break;
  	}
	return i;
 }

static int fullornot(uint32_t index){
	uint32_t i;
	for(i=0;i<WAY_SIZE;i++){
		if(!l1cache.set[index].line[i].validbit)
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
	if(ans<8){
		memcpy(data,l1cache.set[in].line[ans].d+of,BURST_LEN);
 	}
	else{
		ans=fullornot(in);
 		if(ans>=8){
			srand(time(0));
			ans=rand()%WAY_SIZE;
		}
		l1cache.set[in].line[ans].validbit=true;
		l1cache.set[in].line[ans].tag=ta;
		int i;
		for(i=0;i<BLOCK_SIZE;i++)
			l1cache.set[in].line[ans].d[i]=l2read((addr&(~MASK))+i,1);
		memcpy(data,l1cache.set[in].line[ans].d+of,BURST_LEN);
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
		memcpy_with_mask(l1cache.set[in].line[ans].d+of,data,BURST_LEN,mask);
	}
	return;
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
