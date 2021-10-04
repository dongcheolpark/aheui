#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 1000

//debug_mode == 1
//release_mode == 0
#define DEBUG_LEVEL 1
#define han_byte 3

typedef unsigned int han;

//<variables>

char choseong[19][4] = {"ㄱ","ㄲ","ㄴ","ㄷ","ㄸ","ㄹ","ㅁ","ㅂ","ㅃ","ㅅ","ㅆ","ㅇ","ㅈ","ㅉ","ㅊ","ㅋ","ㅌ","ㅍ","ㅎ"};
char jungseong[21][4] = {"ㅏ","ㅐ","ㅑ","ㅒ","ㅓ","ㅔ","ㅕ","ㅖ","ㅗ","ㅘ","ㅙ","ㅚ","ㅛ","ㅜ","ㅝ","ㅞ","ㅟ","ㅠ","ㅡ","ㅢ","ㅣ"};
char jongseong[29][4] = {"X","ㄱ","ㄲ","ㄳ","ㄴ","ㄵ","ㄶ","ㄷ","ㄹ","ㄺ","ㄻ","ㄼ","ㄽ","ㄾ","ㄿ","ㅀ","ㅁ","ㅂ","ㅄ","ㅅ","ㅆ","ㅇ","ㅈ","ㅊ","ㅋ","ㅌ","ㅍ","ㅎ"};

/* hangul error
typedef enum choseong {ㄱ,ㄲ,ㄴ,ㄷ,ㄸ,ㄹ,ㅁ,ㅂ,ㅃ,ㅅ,ㅆ,ㅇ,ㅈ,ㅉ,ㅊ,ㅋ,ㅌ,ㅍ,ㅎ}choseong;
typedef enum jungseong {ㅏ,ㅔ,ㅑ,ㅒ,ㅓ,ㅔ,ㅕ,ㅖ,ㅗ,ㅘ,ㅙ,ㅚ,ㅛ,ㅜ,ㅝ,ㅞ,ㅟ,ㅠ,ㅡ,ㅢ,ㅣ}jungseong;
typedef enum jongseong {X,ㄱ,ㄲ,ㄳ,ㄴ,ㄵ,ㄶ,ㄷ,ㄹ,ㄺ,ㄻ,ㄼ,ㄽ,ㄾ,ㄿ,ㅀ,ㅁ,ㅂ,ㅄ,ㅅ,ㅆ,ㅇ,ㅈ,ㅊ,ㅋ,ㅌ,ㅍ,ㅎ}jongseong;
*/


typedef struct combi {
	char * cho;
	char * jung;
	char * jong;
}combi;

//</variables>

//<header>

bool help(int argc,char * argv[]);

han ** transfer_han(char *);
combi ** tranfer_combi(han **);
void run_aheui(combi **);

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
	fp = fopen(FilePath,"r");
	char buffer[MAX] = { 0, };

	fread(buffer,1,MAX,fp);

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

	for(int i = 0;i<MAX;i++) {
		if(buffer[i] == '\0') {
			buffer_size = i;
			if(width == 0) width = i;
			break;
		}
		if(buffer[i] == '\n') {
			enter_couter++;
			if(width == 0) width = i;
		}
	}

	height = enter_couter+1;
	width /= han_byte;
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
		
};

//</functions>