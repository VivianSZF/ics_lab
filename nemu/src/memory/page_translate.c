#include "common.h"
#include "memory/memory.h"
#include "../../lib-common/x86-inc/mmu.h"
#include "cpu/reg.h"

typedef union{
	struct{
		uint32_t offset :12;
		uint32_t page   :10;
		uint32_t dir    :10;
	};
	uint32_t val;
}lnad;

hwaddr_t page_translate(lnaddr_t addr){
	if(!(cpu.cr0.protect_enable==1&&cpu.cr0.paging==1))
		return addr;
	lnad lnaddr;
	lnaddr.val=addr;
	PDE dir;
	dir.val=hwaddr_read((cpu.cr3.page_directory_base<<12)+lnaddr.dir*4,4);
	printf("0x%x",dir.val);
	if(dir.present==0)
		panic("The page is empty in %x/n",addr);
	PTE pagetable;
	pagetable.val=hwaddr_read((dir.page_frame<<12)+lnaddr.page*4,4);
	assert(pagetable.present==1);
	return (pagetable.page_frame<<12)+lnaddr.offset;
}


