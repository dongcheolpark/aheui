#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 1000

typedef short han;

//<header>

bool help(int argc,char * argv[]);

han ** transfer_han(char *);

int main(int argc,char * argv[]) {

	if(help(argc,argv)) return 1;

	FILE * fp;
	char * FilePath = argv[1];

	fp = fopen(FilePath,"r");
	char buffer[MAX] = { 0, };

	fread(buffer,1,MAX,fp);

	han ** data = transfer_han(buffer);

	fclose(fp);
	return 0;
}

han ** transfer_han(char * buffer) {
	han ** data;
	int enter_couter=0;
	int width = 0,height = 0;
	int buffer_size = 0;
	for(int i = 0;i<MAX;i++) {
		if(buffer[i] == EOF) {
			buffer_size = i;
			break;
		}
		else if(buffer[i] == 0x0A) {
			enter_couter++;
			width = i;
		}
	}
	height = enter_couter+1;
	data = (han**)malloc(sizeof(han*) * (height));
	for(int i = 0;i<height;i++) {
		data[i] = (han *)malloc(sizeof(han) * (width));
	}
	for(int i = 0;i<buffer_size;i++) {

	}
}

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