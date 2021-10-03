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

//<header>

bool help(int argc,char * argv[]);

han ** transfer_han(wchar_t *);

//</header>

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
	wchar_t buffer[MAX] = { 0, };

	fread(buffer,1,MAX,fp);

	han ** data = transfer_han(buffer);

	fclose(fp);
	return 0;
}

//<functions>

han ** transfer_han(wchar_t * buffer) {
	han ** data;
	int enter_couter=0;
	int width = 0,height = 0;
	int buffer_size = 0;

	for(int i = 0;i<MAX;i++) {
		if(buffer[i] == '\0') {
			buffer_size = i;
			break;
		}
		if(buffer[i] == '\n') {
			enter_couter++;
			if(width == 0) width = i;
		}
	}

	height = enter_couter+1;
	width /= han_byte;
	printf("%d %d %d\n",height,width,buffer_size);
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
		data[r][c] += buffer[i];
		i++;
		data[r][c] <<= 8;
		data[r][c] += buffer[i];
		i++;
		data[r][c] <<= 8;
		data[r][c] += buffer[i];
	}
	return data;
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

//</functions>