#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t l1read(hwaddr_t,size_t);
void l1write(hwaddr_t,size_t,uint32_t);
uint32_t l2read(hwaddr_t,size_t);
void l2write(hwaddr_t,size_t,uint32_t);
lnaddr_t seg_translate(swaddr_t ,size_t, uint8_t);
hwaddr_t page_translate(lnaddr_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
//	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	return l1read(addr, len)&(~0u>>((4-len)<<3));
} 

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
//	dram_write(addr, len, data);
	l1write(addr,len,data);
	l2write(addr,len,data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	assert(len==1||len==2||len==4);
	//if(addr>){
	//	assert(0);
	//}
	//else{
		hwaddr_t hwaddr=page_translate(addr);
		return hwaddr_read(hwaddr, len);
	//}
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
	lnaddr_t lnaddr=seg_translate(addr,len,sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr=seg_translate(addr,len,sreg);
	lnaddr_write(lnaddr, len, data);
}

