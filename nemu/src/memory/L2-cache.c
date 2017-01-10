#include "cache.h"
#include<stdio.h>

#define WAY_SIZE 16
#define BLOCK_SIZE (1<<L2_OFFSET)
#define INDEX_SIZE (1<<L2_INDEX)
#define MASK (BLOCK_SIZE-1)
#define HW_MEM_SIZE (1<<27)

typedef struct {
	uint8_t data[WAY_SIZE][BLOCK_SIZE];
	uint32_t tag[WAY_SIZE];
	bool validbit[WAY_SIZE];
	bool dirtybit[WAY_SIZE];
} L2_cache;



L2_cache l2cache[INDEX_SIZE];

uint32_t dram_read(hwaddr_t addr, size_t len);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);

void init_l2cache(){
	memset(l2cache, 0, sizeof(l2cache));
}
/*
static int hitornot(uint32_t index,uint32_t tag){
	uint32_t i;
	for(i=0;i<WAY_SIZE;i++){
		if(l2cache[index].validbit[i]&&l2cache[index].tag[i]==tag)
				break;
  	}
	return i;
 }

static int fullornot(uint32_t index){
	uint32_t i;
 	for(i=0;i<WAY_SIZE;i++){
		if(!l2cache[index].validbit[i])
			break;
	}
	return i;
}
*/
static void write_back(uint32_t index,uint32_t way){
	if(l2cache[index].dirtybit[way]){
		uint32_t i;
		hwaddr_t addr=(l2cache[index].tag[way]<<(L2_OFFSET+L2_INDEX))+(index<<(L2_OFFSET));
		for(i=0;i<BLOCK_SIZE;i++)
			dram_write((addr&(~MASK))+i,1,l2cache[index].data[way][i]);
	}
}

static void l2set_read(hwaddr_t addr, void*data){
	Assert(addr<HW_MEM_SIZE,"physical address %x is outside of the physical memory!",addr);
	cache_addr ad;
	ad.addr=addr&~BURST_MASK;
	uint32_t of=ad.offset2;
	uint32_t in=ad.index2;
	uint32_t ta=ad.tag2;

	bool full=true;
	bool find=false;
	uint32_t i,k;
	for (i=0; i<WAY_SIZE; i++) {
		if (l2cache[in].validbit[i]) {
			if (l2cache[in].tag[i]==ta) {
				memcpy(data, l2cache[in].data[i]+of, BURST_LEN);
				find=true;
				break;
			}
		}
		else {
			full=false;
			k=i;
		}
	}
	if (find==false) {
		if (full) { 
			srand(time(0));
			k=rand()%WAY_SIZE;
			write_back(in, k);
		}
		l2cache[in].validbit[k]=true;
		l2cache[in].dirtybit[k]=false;
		l2cache[in].tag[k]=ta;
		int j;
		for(j=0;j<BLOCK_SIZE;j++)
			l2cache[in].data[k][j]=dram_read((addr&(~MASK))+j,1);
		memcpy(data,l2cache[in].data[k]+of,BURST_LEN);

	}






} 
uint32_t l2read(hwaddr_t addr,size_t len){
	uint32_t offset=addr&BURST_MASK;
	uint8_t temp[2*BURST_LEN];
	l2set_read(addr,temp);
	if(offset+len>BURST_LEN){
		l2set_read(addr+BURST_LEN,temp+BURST_LEN);
	}
	return unalign_rw(temp+offset,4);
}

static void l2set_write(hwaddr_t addr,void *data,uint8_t *mask){
	Assert(addr<HW_MEM_SIZE,"phsical address %x is outside of the physical memory!",addr);
	cache_addr ad;
	ad.addr=addr&~BURST_MASK;
	uint32_t of=ad.offset2;
	uint32_t in=ad.index2;
	uint32_t ta=ad.tag2;


/*	
	uint32_t ans=hitornot(in,ta);
	if(ans<16){
		memcpy_with_mask(l2cache[in].data[ans]+of,data,BURST_LEN,mask);
		l2cache[in].dirtybit[ans]=true;
	}
	else{
		ans=fullornot(in);
		if(ans>=8){
			srand(time(0));
			k=rand()%WAY_SIZE;
			write_back(in,ans);
		}
		l2cache[in].validbit[ans]=true;
		l2cache[in].dirtybit[ans]=false;
		l2cache[in].tag[ans]=ta;
		int i;
		for(i=0;i<BLOCK_SIZE;i++)
			l2cache[in].data[ans][i]=dram_read((addr&(~MASK))+i,1);
		memcpy_with_mask(l2cache[in].data[k]+of,data,BURST_LEN,mask);
	}
	
*/
	uint32_t i, k;
	bool find=false, full=true;
	for (i=0; i<WAY_SIZE; i++) {
		if (l2cache[in].validbit[i]) {
			if (l2cache[in].tag[i]==ta) {
				memcpy_with_mask(l2cache[in].data[i] +of, data, BURST_LEN, mask);
				find=true;
				l2cache[in].dirtybit[i]=true;
				break;
			}
		}
		else {
			full=false;
			k=i;
		}
	}
	if (find==false) {
		if (full) {
			srand(time(0));
			k=rand()%WAY_SIZE;
			write_back(in, k);
		}
		l2cache[in].validbit[k]=true;
		l2cache[in].dirtybit[k]=false;
		l2cache[in].tag[k]=ta;
		int j;
		for(j=0;j<BLOCK_SIZE;j++)
			l2cache[in].data[k][j]=dram_read((addr&(~MASK))+j,1);

		memcpy_with_mask(l2cache[in].data[k] +of, data, BURST_LEN, mask);
	}




}
void l2write(hwaddr_t addr,size_t len,uint32_t data){
	uint32_t offset=addr&BURST_MASK;
	uint8_t temp[2*BURST_LEN];
	uint8_t mask[2*BURST_LEN];
	memset(mask,0,2*BURST_LEN);
	*(uint32_t*)(temp+offset)=data;
	memset(mask+offset,1,len);
	l2set_write(addr,temp,mask);
	if(offset+len>BURST_LEN){
		l2set_write(addr+BURST_LEN,temp+BURST_LEN,mask+BURST_LEN);
	}
}
