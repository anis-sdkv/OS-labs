#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool println(FILE *f){
	int c = '\0';
	while (c != '\n') {
		c = fgetc(f); 
		if (ferror(f)) { 
        		perror("File read error!\n"); 
        		return errno;
        	};
		
		if (c == EOF) return false;
		fputc(c, stdout);
		if (ferror(stdout)) { 
        		perror("File write error!\n"); 
        		return errno;
        	};
		
	};
	return true;
};

int main(int argc, char *argv[]){
	if (argc != 3){
		printf("Using: <file name> <N number>\n");
		return 1;
	};
	
	
	FILE *f = fopen(argv[1], "r");
	if (f == NULL){
		perror("fopen failed!");
		return errno;
	};
	
	int n = atoi(argv[2]);
	
	
	int lineCounter = 0;
	
	while (println(f)) {
		if(n > 0 && ++lineCounter % n == 0) {
			if (fgetc(stdin) == 0x06) break;
		}
			
	};
	
	fclose(f);
	return 0;
};
