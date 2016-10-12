#include "nemu.h"

  /* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

  enum { 
	NOTYPE = 256, EQ, NEQ, NUM, DEREF, NEG, AND, OR ,NOT,OBJECT
	
	/* TODO: Add more token types */

} ;

static struct rule {   
	char *regex;
	int token_type;
	int level;
	int sord;  //single or double operation
} rules[] = {

 	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */  

	{" +",	NOTYPE,6,0},				// spaces
	{"\\+", '+',3,0},					// plus
	{"-", '-',3,0},                    //minus
	{"\\*", '*',4,0},                    //mutiply
	{"/", '/',4,0},                    //divide
	{"\\(", '(',6,0},				    //zuokuohao
	{"\\)", ')',6,0},                    //youkuohao
	{"0x[0-9a-fA-F]+|[0-9]+|\\$[a-z]+", NUM,6,0},  //number
	{"==", EQ,2,0},                      //equal
	{"!=", NEQ,2,0},                     //not equal
	{"&&", AND,1,0},                    //logic and
	{"\\|\\|", OR,1,0},                 //logic or
	{"\\!", NOT,2,1},                  //logic not
	{"[a-zA-Z]|[a-zA-Z][a-zA-Z0-9_]+", OBJECT,6,0},  //object
} ;
 
#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
   */
 void init_regex() {
	int i;
	char error_msg[128];
	int ret;

  	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
 		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
 		}
	}
}

typedef struct token {
	int type;
	char str[32];
	int level;
	int sord;
}   Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i,j;
	regmatch_t pmatch;
	
	nr_token = 0;

 	while(e[position] != '\0') {
		/* Try all rules one by one. */
 		for(i = 0; i < NR_REGEX; i ++) {
 			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;
					
				
				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
 				 */
				tokens[nr_token].type=rules[i].token_type;
				tokens[nr_token].level=rules[i].level;
				tokens[nr_token].sord=rules[i].sord;
 				switch(rules[i].token_type) {
					case NOTYPE:
						nr_token--;break;
					case '+':case '/':case '(':case ')':case EQ:case NEQ:case AND:case OR:case NOT:
						break;
					case '-':
 						if(nr_token==0||(tokens[nr_token-1].type!=NUM && tokens[nr_token-1].type!=')')){
								tokens[nr_token].type=NEG;
								tokens[nr_token].level=5;
								tokens[nr_token].sord=1;
						} 
						break;
                    case '*':
 						if(nr_token==0||(tokens[nr_token-1].type!=NUM && tokens[nr_token-1].type!=')')){
								tokens[nr_token].type=DEREF;
								tokens[nr_token].level=5;
								tokens[nr_token].sord=1;
 						}
						break;
					case NUM:
						if(substr_len>=32){
							for(j=0;j<31;j++)
								tokens[nr_token].str[j]=substr_start[j];
							tokens[nr_token].str[31]='\0';
 						}
						else
 						{
							for(j=0;j<substr_len;j++)
								tokens[nr_token].str[j]=substr_start[j];
							tokens[nr_token].str[substr_len]='\0';
						}
						break;
					case OBJECT:
						strncpy(tokens[nr_token].str,substr_start,substr_len);

						tokens[nr_token].str[substr_len]='\0';
						break;
					default: panic("please implement me");
 				}
				nr_token++;
				break;
  			}
  		}

 	 	if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
  	}

	return true; 
 }  

int check_parenthese(int p,int q){
	int i;
	int sum=0;
	if(tokens[p].type=='('&&tokens[q].type==')'){
		for(i=p+1;i<q;i++){
			if(tokens[i].type=='(')
				sum++;
			if(tokens[i].type==')')
				sum--;
			if(sum<0)
				return false;
  		}
  	}
	else
	{
		return false;
  	}
	if(sum==0) 
		return true;
	else
		return false;
}  

bool pand=true;

uint32_t domiop(int p,int q){
	int min=6;
	int i,sum=0;
	for(i=p;i<=q;i++)
	{
		if(tokens[i].type=='(') sum++;
		if(tokens[i].type==')') sum--;
		if(sum!=0) continue;
		if(tokens[i].level<min)
			min=tokens[i].level;
 	}
	for(i=q;i>=p;i--)
	{
		if(tokens[i].type=='(') sum++;
		if(tokens[i].type==')') sum--;
		if(sum!=0) continue;
		if(tokens[i].level==min){
			if(tokens[i].level==5){
				while(i>p&&tokens[i-1].level==5){
					i--;
				}
			}
			return i;
		}
	}
	pand=false;
	return 0;
}

extern int get_object(char *str);

uint32_t eval(int p, int q){
	uint32_t val=0;
	if(pand==false) return 0;
	if(p>q){
		pand=false;
		return 0;
		}
	else if(p==q)
	{
		if(tokens[p].type==OBJECT){
			val=get_object(tokens[p].str);
			printf("0x%x\n",val);
			if(val==-1){
				pand=false;
				return 0;
			}
		}
		else if(tokens[p].type==NUM)
		{
			int i;
			if(tokens[p].str[0]=='$'){
				if(strcmp(tokens[p].str,"$eax")==0) return cpu.eax;
				else if(strcmp(tokens[p].str,"$ecx")==0) return cpu.ecx;
				else if(strcmp(tokens[p].str,"$edx")==0) return cpu.edx;
				else if(strcmp(tokens[p].str,"$ebx")==0) return cpu.ebx;
				else if(strcmp(tokens[p].str,"$esp")==0) return cpu.esp;
				else if(strcmp(tokens[p].str,"$ebp")==0) return cpu.ebp;
				else if(strcmp(tokens[p].str,"$esi")==0) return cpu.esi;
				else if(strcmp(tokens[p].str,"$edi")==0) return cpu.edi;
				else if(strcmp(tokens[p].str,"$eip")==0) return cpu.eip;
				else{
					pand=false;
					return pand;
 				}
  			}
			else if(tokens[p].str[1]=='x'){
				for(i=2;i<strlen(tokens[p].str);i++){
					if(tokens[p].str[i]>='0'&&tokens[p].str[i]<='9')
						val=val*16+tokens[p].str[i]-'0';
					if(tokens[p].str[i]>='a'&&tokens[p].str[i]<='f')
						val=val*16+tokens[p].str[i]-'a'+10;
					if(tokens[p].str[i]>='A'&&tokens[p].str[i]<='F')
						val=val*16+tokens[p].str[i]-'A'+10;
 				}
 				}
			else
				{
					sscanf(tokens[p].str,"%u",&val);
  				}
			
			return val;
 		}
		else
  		{
			pand=false;
			return 0;
		}
 	}
 	else if(check_parenthese(p,q)==true){
		return eval(p+1,q-1);
 	}
	else{
		int op=domiop(p,q);
		if(pand==false) return 0;
		uint32_t val1=0,val2=0;
 		if(tokens[op].sord==1){
			val1=eval(op+1,q);
		}
		else
 		{
			val1=eval(p,op-1);
			val2=eval(op+1,q);
 		}
 		switch(tokens[op].type){
			case '+':
				return val1+val2;
			case '-':
				return val1-val2;
			case '*':
				return val1*val2;
			case '/':
				return val1/val2;
			case EQ:
				return val1==val2;
			case NEQ:
				return val1!=val2;
			case AND:
				return val1&&val2;
			case OR:
				return val1||val2;
			case NOT:
				return !val1;
			case NEG:
				return -val1;
			case DEREF:
				return swaddr_read(val2,4);
			case NUM:case OBJECT:
				pand=false;
				return 0;
			default:assert(0);
	 	}
   	}
	return 0;
}  


uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
 	}

	/* TODO: Insert codes to evaluate the expression. */
	//uint32_t k=eval(0,nr_token-1);
	//printf("%u\n",k);	
	return eval(0,nr_token-1);	
 }

