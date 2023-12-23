#include <errno.h>
#include <stdio.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char *argv[]){
	if (argc != 3){
		printf("Usage: <source file> <destination file>\n");
		return 1;
	};
	
	
	FILE *src = fopen(argv[1], "r");
	if (src == NULL){
		perror("Source file open failed!");
		return errno;
	};
	
	struct stat src_stat;
	stat(argv[1], &src_stat);
	
	
	FILE *dst = fopen(argv[2], "w");
	if (dst == NULL){
		perror("Destination file open failed!");
		return errno;
	};
	
	chmod(argv[2], src_stat.st_mode);
	
	int c = '\0';
	while (true) {
		c = fgetc(src);
		if (ferror(src)) { 
        		perror("File read error!\n"); 
        		return errno;
        	};
        	   
		if (c == EOF) break;
		fputc(c, dst);
		if (ferror(src)) { 
        		perror("File write error!\n"); 
        		return errno;
        	};
	};
	
	fclose(src);
	fclose(dst);
	return 0;
};
