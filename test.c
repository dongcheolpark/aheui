#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX 1000

bool help(int argc,char * argv[]);

int main(int argc,char * argv[]) {

	if(help(argc,argv)) return 1;

	FILE * fp;
	char * FilePath = argv[1];

	fp = fopen(FilePath,"r");
	char buffer[MAX] = { 0, };

	fread(buffer,1,MAX,fp);
	printf("%s",buffer);

	fclose(fp);

	return 0;
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