#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

 void init_wp_pool() {
	int i;
  	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(){
	if(free_==NULL) 
		assert(0);
	WP *new_=free_;
	free_=free_->next;
	new_->next=head;
	head=new_;
	printf("New watchpoint %d\n",new_->NO);
	return new_;
 }

void free_wp(char *str){
	WP *p=head;
	int n;
	sscanf(str,"%d",&n);
	if(p->NO==n){
		head=p->next;
		p->next=free_;
		free_=p;
		printf("Delete watchpoint %d\n",n);
	}
	else{
		WP *q=p->next;
		while(q!=NULL){
			if(q->NO==n){
				p->next=q->next;
				q->next=free_;
				free_=q;
				printf("Delete watchpoint %d\n",n);
				break;
			}
			else{
				p=q;
				q=q->next;
			}
		}
	}
}	

bool check_wp(){
	WP *p=head;
	bool success=true;
	bool pan=false;
	while(p!=NULL){
		uint32_t val=expr(p->expr,&success);
		if(val!=p->value){
			printf("Stop at watchpoint %d\n",p->NO);
			printf("Old value = %u\n",p->value);
			printf("New value = %u\n",val);
			p->value=val;
			pan=true;
		}
		p=p->next;
	}
	return pan;
}

void print_wp(){
	WP *p=head;
	while(p!=NULL){
		printf("Num     Type    Disp  Enb  Address\n");
		printf("%-3d watchpoint keep   y    %-7s\n",p->NO,p->expr);
		p=p->next;
	}
}







