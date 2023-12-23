#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>


void printFiles(char* path){
	DIR *dir = opendir(path);
	if (dir == NULL){
		perror("Directory open failed!");
		exit(errno);
	};
	
	int counter = 0;
	
	struct dirent *d;
	printf("Directory: %s\n", path);
	while ((d = readdir(dir)) != NULL){
		printf("\t%i. %s\n", ++counter, d -> d_name);
	};
	
	if (closedir(dir) == -1) {
    		perror("Directory close failed!");
    		exit(errno);
	}
	printf("\n");
	
};

int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Usage: <directory path>\n");
		return 1;
	};
	
	printFiles(argv[1]);
	
	printFiles(".");
	
	return 0;
};
