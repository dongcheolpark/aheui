#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100000

//debug_mode == 1
//release_mode == 0
#define DEBUG_LEVEL 1
#define HAN_BYTE    3
#define STACK_COUNT 26
#define QUEUE_COUNT 1

#define CHO_SIZE    19
#define JUNG_SIZE   21
#define JONG_SIZE   28

#define reverse(X) -X

typedef unsigned int han;

typedef struct combi {
	char * cho;
	char * jung;
	char * jong;
}combi;

typedef struct node {
	int data;
	struct node * next;
}node;

typedef struct stack {
	node * data;
}stack;

typedef struct queue {
	node * data;
}queue;

//<variables>

char choseong[19][4] = {"ㄱ","ㄲ","ㄴ","ㄷ","ㄸ","ㄹ","ㅁ","ㅂ","ㅃ","ㅅ","ㅆ","ㅇ","ㅈ","ㅉ","ㅊ","ㅋ","ㅌ","ㅍ","ㅎ"};
char jungseong[21][4] = {"ㅏ","ㅐ","ㅑ","ㅒ","ㅓ","ㅔ","ㅕ","ㅖ","ㅗ","ㅘ","ㅙ","ㅚ","ㅛ","ㅜ","ㅝ","ㅞ","ㅟ","ㅠ","ㅡ","ㅢ","ㅣ"};
char jongseong[28][4] = {"X","ㄱ","ㄲ","ㄳ","ㄴ","ㄵ","ㄶ","ㄷ","ㄹ","ㄺ","ㄻ","ㄼ","ㄽ","ㄾ","ㄿ","ㅀ","ㅁ","ㅂ","ㅄ","ㅅ","ㅆ","ㅇ","ㅈ","ㅊ","ㅋ","ㅌ","ㅍ","ㅎ"};
int jongseong_line[28] = {0,2,4,4,2,5,5,3,5,7,9,9,7,9,9,8,4,4,6,2,4,0,3,4,3,4,4,0};

stack * st[26];
queue * qu;

// sw == 0 -> stack, 1-> queue
int sw = 0;

/* hangul error
typedef enum choseong {ㄱ,ㄲ,ㄴ,ㄷ,ㄸ,ㄹ,ㅁ,ㅂ,ㅃ,ㅅ,ㅆ,ㅇ,ㅈ,ㅉ,ㅊ,ㅋ,ㅌ,ㅍ,ㅎ}choseong;
typedef enum jungseong {ㅏ,ㅔ,ㅑ,ㅒ,ㅓ,ㅔ,ㅕ,ㅖ,ㅗ,ㅘ,ㅙ,ㅚ,ㅛ,ㅜ,ㅝ,ㅞ,ㅟ,ㅠ,ㅡ,ㅢ,ㅣ}jungseong;
typedef enum jongseong {X,ㄱ,ㄲ,ㄳ,ㄴ,ㄵ,ㄶ,ㄷ,ㄹ,ㄺ,ㄻ,ㄼ,ㄽ,ㄾ,ㄿ,ㅀ,ㅁ,ㅂ,ㅄ,ㅅ,ㅆ,ㅇ,ㅈ,ㅊ,ㅋ,ㅌ,ㅍ,ㅎ}jongseong;
*/

//</variables>

//<header>

bool help(int argc,char * argv[]);

han ** transfer_han(char *);
combi ** tranfer_combi(han **);
void run_aheui(combi **);
//stack
void stack_pop(stack *); 
int stack_top(stack *);
void stack_push(stack *,int);
int stack_size(stack *);
//queue
void queue_pop(queue *); 
int queue_top(queue *);
void queue_push(queue *,int);
int queue_size(queue *);

void * data_structure_index(char *);
int jong_num(char *);

//</header>

//<variables>
int width = 0,height = 0;
//</variables>

int main(int argc,char * argv[]) {
	FILE * fp;
	char * FilePath = NULL;
#if DEBUG_LEVEL == 0
	if(help(argc,argv)) return 1;
	FilePath = argv[1];
#endif

#if DEBUG_LEVEL == 1
	FilePath = "test.ahui";
#endif
	//read file
	fp = fopen(FilePath,"r");
	char buffer[MAX] = { 0, };

	fread(buffer,1,MAX,fp);

	//transrate
	han ** data = transfer_han(buffer);

	combi ** data_combi = tranfer_combi(data);

	for(int i = 0;i<height;i++) {
		free(data[i]);
	}
	//free memories of data;
	free(data);
	fclose(fp);

	/*for(int i = 0;i<height;i++) {
		for(int j = 0;j<width;j++) {
			printf("%s %s %s\n",data_combi[i][j].cho,data_combi[i][j].jung,data_combi[i][j].jong);
		}
		puts("");
	}*/

	for(int i = 0;i<STACK_COUNT;i++) {
		st[i] = (stack *)malloc(sizeof(stack));
		st[i]->data = NULL;
	}	
	qu = (queue *)malloc(sizeof(queue));

	run_aheui(data_combi);

	return 0;
}

//<functions>

bool help(int argc,char * argv[]) {
	if(argc == 1) {
		puts("aheui : fatal error: no input files");
		puts("compilation terminated");
		return 1;
	}
	else if(strcmp(argv[1],"--help" ) == 0) {
		puts("Usage : ahui.out file...");
		return 0;
	}
	else return 0;
}

han ** transfer_han(char * buffer) {
	han ** data;
	int enter_couter=0;
	int buffer_size = 0;
	int tmp = 0;
	for(int i = 0;i<MAX;i++,tmp++) {
		if(buffer[i] == '\0') {
			buffer_size = i;
			if(width < tmp) {
				width = tmp;
			} 
			tmp = 0;
			break;
		}
		if(buffer[i] == '\n') {
			enter_couter++;
			if(width < tmp) {
				width = tmp;
			} 
			tmp = 0;
		}
	}

	height = enter_couter+1;
	width /= HAN_BYTE;
	//printf("%d %d %d\n",height,width,buffer_size);
	data = (han**)malloc(sizeof(han*) * (height));
	for(int i = 0;i<height;i++) {
		data[i] = (han *)malloc(sizeof(han) * (width));
	}

	int r = 0,c = 0;
	for(int i = 0;i<buffer_size;i++) {
		if(buffer[i] == '\x0A') {
			r++;
			c = 0;
			continue;
		}
		//utf+8 to unicode
		int tmp = (unsigned char)buffer[i];
		tmp -= 14<<4; 
		data[r][c] = tmp;
		i++;
		data[r][c]<<=6;
		tmp = (unsigned char)buffer[i];
		tmp -= 2<<6;
		data[r][c]+=tmp;
		i++;
		data[r][c]<<=6;
		tmp = (unsigned char)buffer[i];
		tmp -= 2<<6;
		data[r][c]+=tmp;
		c++;
	}
	return data;
}

combi ** tranfer_combi(han ** data) {
	combi ** data_combi;
	data_combi = (combi **)malloc(sizeof(combi *)*height);
	for(int i = 0;i<width;i++) {
		data_combi[i] = (combi *)malloc(sizeof(combi) * width);
	}
	//malloc memoires

	//jaso seperate
	for(int i = 0;i<height;i++) {
		for(int j = 0;j<width;j++) {
			int cho = (data[i][j]-44032)/(21*28);
			int jung = (data[i][j]-44032-(cho*21*28))/28;
			int jong  = (data[i][j]-44032-(cho*21*28)-(jung*28));
			data_combi[i][j].cho = choseong[cho];
			data_combi[i][j].jung = jungseong[jung];
			data_combi[i][j].jong = jongseong[jong];
			//printf("%s %s %s\n",data_combi[i][j].cho,data_combi[i][j].jung,data_combi[i][j].jong);
		}
	}
	return data_combi;
}

void run_aheui(combi ** data) {
	int i = 0,j = 0;
	bool turn = 0;
	void * data_set = st[0];
	void (* data_set_pop)(void *);
	int (* data_set_top)(void *);
	void (* data_set_push)(void *,int);
	int (* data_set_size)(void *);
	int prog_i = 0,prog_j = 0;
	while(1) {
		turn = 0;
		//printf("%d %d\n",i,j);
		if(sw == 0) {
			data_set_pop = stack_pop;
			data_set_top = stack_top;
			data_set_push = stack_push;
			data_set_size = stack_size;
		}
		else {
			data_set_pop = queue_pop;
			data_set_top = queue_top;
			data_set_push = queue_push;
			data_set_size = queue_size;
		}
		//ㅇ set
		if(strcmp(data[i][j].cho,"ㅇ") == 0) {
			//do nothing
		}
		else if(strcmp(data[i][j].cho,"ㅎ") == 0) {
			break;
		}
		//ㄷ set
		else if(strcmp(data[i][j].cho,"ㄷ") == 0) {
			if(data_set_size(data_set) < 2) {
				turn = 1;
				goto TRANS;
			}
			int a,b;
			a = data_set_top(data_set);
			data_set_pop(data_set);
			b = data_set_top(data_set);
			data_set_pop(data_set);
			data_set_push(data_set,a+b);
		}
		else if(strcmp(data[i][j].cho,"ㄸ") == 0) {
			if(data_set_size(data_set) < 2) {
				turn = 1;
				goto TRANS;
			}
			int a,b;
			a = data_set_top(data_set);
			data_set_pop(data_set);
			b = data_set_top(data_set);
			data_set_pop(data_set);
			data_set_push(data_set,a*b);
		}
		else if(strcmp(data[i][j].cho,"ㅌ") == 0) {
			if(data_set_size(data_set) < 2) {
				turn = 1;
				goto TRANS;
			}
			int a,b;
			a = data_set_top(data_set);
			data_set_pop(data_set);
			b = data_set_top(data_set);
			data_set_pop(data_set);
			data_set_push(data_set,b-a);
		}
		else if(strcmp(data[i][j].cho,"ㄴ") == 0) {
			if(data_set_size(data_set) < 2) {
				turn = 1;
				goto TRANS;
			}
			int a,b;
			a = data_set_top(data_set);
			data_set_pop(data_set);
			b = data_set_top(data_set);
			data_set_pop(data_set);
			data_set_push(data_set,b/a);
		}
		else if(strcmp(data[i][j].cho,"ㄹ") == 0) {
			if(data_set_size(data_set) < 2) {
				turn = 1;
				goto TRANS;
			}
			int a,b;
			a = data_set_top(data_set);
			data_set_pop(data_set);
			b = data_set_top(data_set);
			data_set_pop(data_set);
			data_set_push(data_set,b%a);
		}
		//ㅁ set
		else if(strcmp(data[i][j].cho,"ㅁ") == 0) {
			if(data_set_size(data_set) < 1) {
				turn = 1;
				goto TRANS;
			}
			if(strcmp(data[i][j].jong,"ㅇ") == 0) {
				printf("%d",data_set_top(data_set));
				data_set_pop(data_set);
			}
			else if(strcmp(data[i][j].jong,"ㅎ") == 0) {
				printf("%c",data_set_top(data_set));
				data_set_pop(data_set);
			}
		} 
		else if(strcmp(data[i][j].cho,"ㅂ") == 0) {
			if(strcmp(data[i][j].jong,"ㅇ") == 0) {
				int tmp;
				printf("한 정수를 입력하세요 :");
				scanf("%d",&tmp);
				data_set_push(data_set,tmp);
			}
			else if(strcmp(data[i][j].jong,"ㅎ") == 0){
				char c;
				printf("한 문자를 입력하세요 :");
				scanf("%c",&c);
				data_set_push(data_set,c);
			} 
			else {
				data_set_push(data_set,jong_num(data[i][j].jong));
				//printf("%d\n",data_set_top(data_set));
			}
		}
		else if(strcmp(data[i][j].cho,"ㅃ") == 0) {
			data_set_push(data_set,data_set_top(data_set));
		}
		else if(strcmp(data[i][j].cho,"ㅍ") == 0) {
			int a,b;
			a = data_set_top(data_set);
			data_set_pop(data_set);
			b = data_set_top(data_set);
			data_set_pop(data_set);
			data_set_push(data_set,a);
			data_set_push(data_set,b);
		}
		//ㅅ set
		else if(strcmp(data[i][j].cho,"ㅅ") == 0) {
			data_set = data_structure_index(data[i][j].jong);
		}
		else if(strcmp(data[i][j].cho,"ㅆ") == 0) {
			if(data_set_size(data_set) < 1) {
				turn = 1;
				goto TRANS;
			}
			int a = data_set_top(data_set);
			data_set_pop(data_set);
			data_set_push(data_structure_index(data[i][j].jong),a);
		}
		else if(strcmp(data[i][j].cho,"ㅈ") == 0) {
			if(data_set_size(data_set) < 2) {
				turn = 1;
				goto TRANS;
			}
			int a,b;
			a = data_set_top(data_set);
			data_set_pop(data_set);
			b = data_set_top(data_set);
			data_set_pop(data_set);
			data_set_push(data_set,b>=a);
		}
		else if(strcmp(data[i][j].cho,"ㅊ") == 0) {
			if(data_set_size(data_set) < 1) {
				turn = 1;
				goto TRANS;
			}
			int a = data_set_top(data_set);
			if(a == 0) {
				turn =1;
				goto TRANS;
			}
		}

		//move pointer
		TRANS :

		if(strcmp(data[i][j].jung,"ㅏ") == 0) {
			prog_i = 0;
			prog_j = 1;
		}
		else if(strcmp(data[i][j].jung,"ㅓ") == 0) {
			prog_i = 0;
			prog_j = -1;
		}
		else if(strcmp(data[i][j].jung,"ㅗ") == 0) {
			prog_i = -1;
			prog_j = 0;
		}
		else if(strcmp(data[i][j].jung,"ㅜ") == 0) {
			prog_i = 1;
			prog_j = 0;
		}

		else if(strcmp(data[i][j].jung,"ㅑ") == 0) {
			prog_i = 0;
			prog_j = 2;
		}
		else if(strcmp(data[i][j].jung,"ㅕ") == 0) {
			prog_i = 0;
			prog_j = -2;
		}
		else if(strcmp(data[i][j].jung,"ㅛ") == 0) {
			prog_i = -2;
			prog_j = 0;
		}
		else if(strcmp(data[i][j].jung,"ㅠ") == 0) {
			prog_i = 2;
			prog_j = 0;
		}

		else if(strcmp(data[i][j].jung,"ㅡ") == 0) {
			if(prog_j == 0) {
				i -= prog_i;
				j -= prog_j;
				prog_j *= -1;
			}
		}
		else if(strcmp(data[i][j].jung,"ㅣ") == 0) {
			if(prog_i == 0) {
				i -= prog_i;
				j -= prog_j;
				prog_i *= -1;
			}
		}
		else if(strcmp(data[i][j].jung,"ㅢ") == 0) {
			i -= prog_i;
			j -= prog_j;
			prog_i *= -1;
			prog_j *= -1;
		}
		if(turn) {
			i -= prog_i;
			j -= prog_j;
			prog_i *= -1;
			prog_j *= -1;

		}
		else {
			i += prog_i;
			j += prog_j;
		}
	}
};

void stack_pop(stack * s) {
	if(s->data != NULL) {
		node * n = s->data;
		s->data = s->data->next;
		free(n);
	}
	return;
}
int stack_top(stack * s) {
	return s->data->data;
}
void stack_push(stack * s,int data) {
	node * n = (node*)malloc(sizeof(node));
	n->next = NULL;
	n->data = data;
	if(s->data == NULL) {
		s->data = n;
	}
	else {
		n->next = s->data;
		s->data = n;
	}
}

int stack_size(stack * s) {
	int i = 0;
	for(node * n = s->data;n != NULL;i++,n = n->next); 
	return i;
}

void queue_pop(queue * q) {
	if(q->data != NULL) {
		node * prev = NULL;
		for(node * i = q->data;i->next != NULL;i = i->next) {
			prev = i;
		}
		if(prev == NULL) {
			q->data = NULL;
			free(q->data);
		}
		else {
			free(prev->next);
			prev->next = NULL;
		}
	}
}
int queue_top(queue * q) {
	if(q->data != NULL) {
		node * prev = NULL;
		for(node * i = q->data;i->next != NULL;i = i->next) {
			prev = i;
		}
		if(prev == NULL) {
			return q->data->data;
		}
		else {
			return prev->next->data;
		}
	}
	else return -1;
}
void queue_push(queue * q,int data) {
	node * n = (node*)malloc(sizeof(node));
	n->next = NULL;
	n->data = data;
	if(q->data == NULL) {
		q->data = n;
	}
	else {
		n->next = q->data;
		q->data = n;
	}
}
int queue_size(queue * q) {
	int i = 0;
	for(node * n = q->data;n != NULL;i++,n = n->next); 
	return i;
}
void * data_structure_index(char * jong) {
	int index = 0;
	for(int i = 0;i<JONG_SIZE;i++) {
		if(strcmp(jongseong[i],"ㅇ") == 0 ||strcmp(jongseong[i],"ㅎ") == 0) {
			if(strcmp(jong,"ㅇ") == 0) {
				sw = 1;
				return qu;
			}
			else if(strcmp(jong,"ㅎ") == 0) {
				return NULL;
			}
			continue;
		}
		else if(strcmp(jong,jongseong[i]) == 0) {
			sw = 0;
			return st[index];
		}
		index++;
	}
	return NULL;
}

int jong_num(char * _jong) {
	int jong = 0;
	for(int i = 0;i<JONG_SIZE;i++) {
		if(strcmp(_jong,jongseong[i]) == 0) {
			jong = i;
			break;
		}
	}
	return jongseong_line[jong];
}
//</functions>
