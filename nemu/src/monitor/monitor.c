#include "nemu.h"

#define ENTRY_START 0x100000

extern uint8_t entry [];
extern uint32_t entry_len;
extern char *exec_file;

void load_elf_tables(int, char *[]);
void init_regex();
void init_wp_pool();
void init_ddr3();
void init_l1cache();
void init_l2cache();
void init_TLB();
void init_device();
void init_sdl();

FILE *log_fp = NULL;

void init_CR0(){
	cpu.cr0.protect_enable=0;
	cpu.cr0.paging=0;
	int i;
	for(i=0;i<4;i++)
		cpu.segcache[i].valid=false;
	cpu.segcache[1].valid=true;
	cpu.segcache[1].base=0;
	cpu.segcache[1].limit=0xffffffff;
}

static void init_log() {
	log_fp = fopen("log.txt", "w");
	Assert(log_fp, "Can not open 'log.txt'");
}

static void welcome() {
	printf("Welcome to NEMU!\nThe executable is %s.\nFor help, type \"help\"\n",
			exec_file);
}

void init_monitor(int argc, char *argv[]) {
	/* Perform some global initialization */

	/* Open the log file. */
	init_log();

	/* Load the string table and symbol table from the ELF file for future use. */
	load_elf_tables(argc, argv);

	/* Compile the regular expressions. */
	init_regex();

	/* Initialize the watchpoint pool. */
	init_wp_pool();

	init_device();
	init_sdl();

	/* Display welcome message. */
	welcome();
}

#ifdef USE_RAMDISK
static void init_ramdisk() {
	int ret;
	const int ramdisk_max_size = 0xa0000;
	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	Assert(file_size < ramdisk_max_size, "file size(%zd) too large", file_size);

	fseek(fp, 0, SEEK_SET);
	ret = fread(hwa_to_va(0), file_size, 1, fp);
	assert(ret == 1);
	fclose(fp);
}
#endif

static void load_entry() {
	int ret;
	FILE *fp = fopen("entry", "rb");
	Assert(fp, "Can not open 'entry'");

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	ret = fread(hwa_to_va(ENTRY_START), file_size, 1, fp);
	assert(ret == 1);
	fclose(fp);
}

 void restart() {
	/* Perform some initialization to restart a program */
	cpu.CF=0;
	cpu.a1=1;
	cpu.PF=0;
	cpu.a2=0;
	cpu.AF=0;
	cpu.a3=0;
	cpu.ZF=0;
	cpu.SF=0;
	cpu.TF=0;
	cpu.IF=0;
	cpu.DF=0;
	cpu.OF=0;
#ifdef USE_RAMDISK
	/* Read the file with name `argv[1]' into ramdisk. */
	init_ramdisk();
#endif

	/* Read the entry code into memory. */
	load_entry();

	/* Set the initial instruction pointer. */
	cpu.eip = ENTRY_START;

	/* Initialize DRAM. */
	init_ddr3();

	/*Initialize cache. */
	init_l1cache();
	init_l2cache();

	/*Initialize CR0*/
	init_CR0();

	/*Initialize TLB*/
	init_TLB();
}
