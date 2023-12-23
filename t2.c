#include <errno.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Enter the file name!\n");
		return 1;
	};
	
	
	FILE *f = fopen(argv[1], "w");
	if (f == NULL){
		perror("fopen failed!");
		return errno;
	};
	
	int c = '\0';
	while (c != EOF && c != 0x06) {
		c = fgetc(stdin); 
		if (ferror(stdin)) { 
        		perror("File read error!\n"); 
        		return errno;
        	};
		
		fputc(c, f);
		if (ferror(f)) { 
        		perror("File write error!\n"); 
        		return errno;
        	};
	};
	
	fclose(f);
	return 0;
};
