#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t l1read(hwaddr_t,size_t);
void l1write(hwaddr_t,size_t,uint32_t);
uint32_t l2read(hwaddr_t,size_t);
void l2write(hwaddr_t,size_t,uint32_t);
lnaddr_t seg_translate(swaddr_t ,uint8_t);
hwaddr_t page_translate(lnaddr_t);
int is_mmio(hwaddr_t);
uint32_t mmio_read(hwaddr_t,size_t,int);
void mmio_write(hwaddr_t,size_t,uint32_t,int);



/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
    	int map_NO=is_mmio(addr);
	if(map_NO==-1){
		return l1read(addr, len)&(~0u>>((4-len)<<3));
	}
	else{
		return mmio_read(addr,len,map_NO)&(~0u>>((4-len)<<3));
	}
} 

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	int map_NO=is_mmio(addr);
	if(map_NO==-1){
		l1write(addr,len,data);
		l2write(addr,len,data);
	}
 	else{
		mmio_write(addr,len,data,map_NO);
	}
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	assert(len==1||len==2||len==4);
	if((addr&0x7)+len>4096){
		assert(0);
	}
	else{
		hwaddr_t hwaddr=page_translate(addr);
//		printf("0x%x 0x%x\n",addr,hwaddr);
		return hwaddr_read(hwaddr, len);
	}
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	assert(len==1||len==2||len==4);
	//if(addr>){
	//	assert(0);
	//}
	//else{
		hwaddr_t hwaddr=page_translate(addr);
		
		hwaddr_write(hwaddr, len, data);
	//}
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr=seg_translate(addr,sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr=seg_translate(addr,sreg);
	lnaddr_write(lnaddr, len, data);
}

