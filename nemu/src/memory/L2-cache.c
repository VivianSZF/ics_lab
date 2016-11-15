#include "cache.h"

#define WAY_SIZE 16
#define BLOCK_SIZE (1<<L2_OFFSET)
#define INDEX_SIZE (1<<L2_INDEX)
#define MASK (BLOCK_SIZE-1)
#define HW_MEM_SIZE (1<<27)

typedef struct{ 
	uint8_t d[BLOCK_SIZE];
	uint32_t tag;
	bool dirtybit;
	bool validbit;
}LINE;
typedef struct{
	LINE line[WAY_SIZE];
}SET;
typedef struct{
	SET set[INDEX_SIZE];
}L2_cache;

static L2_cache l2cache;

uint32_t dram_read(hwaddr_t addr, size_t len);
uint32_t dram_write(hwaddr_t addr, size_t len, uint32_t data);

void init_l2cache(){
	int i,j;
	for(i=0;i<INDEX_SIZE;i++)
		for(j=0;j<WAY_SIZE;j++)
			l2cache.set[i].line[j].validbit=false;
}

static int hitornot(uint32_t index,uint32_t tag){
	uint32_t i;
	for(i=0;i<WAY_SIZE;i++){
		if(l2cache.set[index].line[i].validbit&&l2cache.set[index].line[i].tag==tag)
				break;
  	}
	return i;
 }

static int fullornot(uint32_t index){
	uint32_t i;
 	for(i=0;i<WAY_SIZE;i++){
		if(!l2cache.set[index].line[i].validbit)
			break;
	}
	return i;
}

static void write_back(uint32_t index,int way,hwaddr_t addr){
	if(l2cache.set[index].line[way].dirtybit){
		uint32_t i;
		for(i=0;i<BLOCK_SIZE;i++)
			dram_write(addr,1,l2cache.set[index].line[way].d[i]);
	}
}

static void l2set_read(hwaddr_t addr, void*data){
	Assert(addr<HW_MEM_SIZE,"physical address %x is outside of the physical memory!",addr);
	cache_addr ad;
	ad.addr=addr&~BURST_MASK;
	uint32_t of=ad.offset2;
	uint32_t in=ad.index2;
	uint32_t ta=ad.tag2;
	uint32_t ans=hitornot(in,ta);
	if(ans<16){
		memcpy(data,l2cache.set[in].line[ans].d+of,BURST_LEN);
	}
	else{
		ans=fullornot(in);
		if(ans>=8){
			srand(time(0));
			ans=rand()%WAY_SIZE;
			write_back(in,ans,addr);
		}
		l2cache.set[in].line[ans].validbit=true;
		l2cache.set[in].line[ans].dirtybit=false;
		l2cache.set[in].line[ans].tag=ta;
		int i;
		for(i=0;i<BLOCK_SIZE;i++)
			l2cache.set[in].line[ans].d[i]=dram_read((addr&(~MASK))+i,1);
		memcpy(data,l2cache.set[in].line[ans].d+of,BURST_LEN);
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
	uint32_t ans=hitornot(in,ta);
	if(ans<16){
		memcpy_with_mask(l2cache.set[in].line[ans].d+of,data,BURST_LEN,mask);
		l2cache.set[in].line[ans].dirtybit=true;
	}
	else{
		ans=fullornot(in);
		if(ans>=8){
			srand(time(0));
			ans=rand()%WAY_SIZE;
			write_back(in,ans,addr);
		}
		l2cache.set[in].line[ans].validbit=true;
		l2cache.set[in].line[ans].dirtybit=true;
		l2cache.set[in].line[ans].tag=ta;
		int i;
		for(i=0;i<WAY_SIZE;i++)
			l2cache.set[in].line[ans].d[i]=dram_read((addr&(~MASK))+i,1);
		memcpy_with_mask(l2cache.set[in].line[ans].d+of,data,BURST_LEN,mask);
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
