#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);
uint32_t expr(char *str,bool *success);
extern WP* new_wp();
extern void free_wp(char *str);
extern void print_wp();

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static int cmd_bt(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Execute single step", cmd_si },
	{ "info", "Print the information about registers and watchpoint", cmd_info },
	{ "x", "scan the memory", cmd_x },
	{ "p", "evaluate the expression", cmd_p},
	{ "w", "set watchpoint", cmd_w},
	{ "d", "delete watchpoint", cmd_d},
	{ "bt", "print the stack frame", cmd_bt}
	/* TODO: Add more commands */

   };

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

  static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

 	if(arg == NULL) {
		/* no argument given */
 		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
  		}
  	}
 	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
   			}
   		}
		printf("Unknown command '%s'\n", arg);
  	}
	return 0;
   }

static int cmd_si(char *args){
	uint32_t n;
	if(args==NULL)
			cpu_exec(1);
	else{
		int pan=sscanf(args,"%u",&n);
		if(pan==1)
			cpu_exec(n);
		else
			printf("Wrong usage of command 'si %s'\n",args);	
  	}
	return 0;
 } 

 static int cmd_info(char *args){
	char buf[5];
	int pan=sscanf(args,"%s",buf);
	if(pan==1)
	{
		if(strcmp(buf,"r")==0)
		{
			printf("eax     0x%x      %u\n",cpu.eax,cpu.eax);
			printf("ecx     0x%x      %u\n",cpu.ecx,cpu.ecx);
			printf("edx     0x%x      %u\n",cpu.edx,cpu.edx);
			printf("ebx     0x%x      %u\n",cpu.ebx,cpu.ebx);
			printf("esp     0x%x      %u\n",cpu.esp,cpu.esp);
			printf("ebp     0x%x      %u\n",cpu.ebp,cpu.ebp);
			printf("esi     0x%x      %u\n",cpu.esi,cpu.esi);
			printf("edi     0x%x      %u\n",cpu.edi,cpu.edi);
			printf("eip     0x%x      %u\n",cpu.eip,cpu.eip);
		} 
		else if (strcmp(buf,"w")==0)
			{
				print_wp();
			} 
		else
	 		{
				printf("Wrong usage of command 'info %s'\n",args);
	 		}
  	}
 	else
	{
		printf("Wrong usage of command 'info %s'\n",args);
		return 0;
  	}
	return 0;	
}	 	

 static int cmd_x(char *args){
	int n;
	uint32_t addr;
	int i;
	char buf[102];
	int pan=sscanf(args,"%d%s",&n,buf);
	if(buf[0]=='x')
 	{
		for(i=100;i>=0;i--)
 		{
			buf[i+1]=buf[i];
		}
		buf[0]='0';
	}
	bool pand=true;
	addr=expr(buf,&pand);
	i=0;
	int t=0;
	if(pan==2)
 	{
		for(i=0;i<n;i++)
		{
			if(t==0||t%4==0)
				printf("0x%x:   ",addr);
			printf("0x%08x   ",swaddr_read(addr,4,S_DS));
			addr=addr+4;
			t++;
			if(t%4==0)
				printf("\n");
	 	}
		if(n%4!=0) printf("\n");
	}
	else
	{
		printf("Wrong usage of command 'x %s'\n",args);
 	}
	return 0;
}

static int cmd_p(char *args){
	if(args==NULL){
		printf("Wrong usage of command p\n");
		return 0;
  	}
	bool pan=true;
	printf("0x%x\n",expr(args, &pan));
	return 0;
 } 

static int cmd_w(char *args){
	if(args==NULL){
		printf("Wrong usage of command w\n");
		return 0;
 	}
	WP *new_=new_wp();
	strcpy(new_->expr,args);
	new_->expr[strlen(args)]='\0';
	bool pan=true;
	new_->value=expr(args,&pan);
	//print_wp();
	return 0;
} 

static int cmd_d(char *args){
	if(args==NULL){
		printf("Wrong usage of command d\n");
		return 0;
 	}
	free_wp(args);
	return 0;
 }

extern char *get_func(int addr, int *pan);

static int cmd_bt(char *args){
	int pan=0,time=0;
	int ebp=cpu.ebp;
	int addr=cpu.eip;
	get_func(addr,&pan);
	if(!pan){
		printf("No stack frame!\n");
		return 0;
	}
	while(ebp!=0){
		//printf("ebp: %x\n",ebp);
		printf("#%d 0x%x in %s(%d %d %d %d)\n",time++,addr,get_func(addr,&pan),swaddr_read(ebp+8,4,S_SS),swaddr_read(ebp+12,4,S_SS),swaddr_read(ebp+16,4,S_SS),swaddr_read(ebp+20,4,S_SS));
		addr=swaddr_read(ebp+4,4,S_SS);
		ebp=swaddr_read(ebp,4,S_SS);
	}
	return 0;
}

void ui_mainloop() {
 	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
 		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
 		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
