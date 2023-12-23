#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

off_t fsize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    perror(filename);
    exit(errno); 
}

char *allocate_offset(int c, size_t n)
{
    char *s = malloc(n + 1);
    memset(s, c, n);
    s[n] = '\0';
    return s;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); 
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


void writeToFile(char* path, char* text){
	FILE *f = fopen(path, "w");
	if (f == NULL){
		perror("fopen failed!");
		exit(errno);
	};
	
	for(int i = 0; text[i] != '\0';i++){
		fputc(text[i], f);
		if (ferror(f)) { 
	       		perror("File write error!\n"); 
	       		exit(errno);
	       	};
	};
        	
        fclose(f);
};


char* printFiles(char* path, int level){

	char *result;

	DIR *dir = opendir(path);
	if (dir == NULL){
		perror(concat("Directory open failed - ", path));
		exit(errno);
	};
	
	char **directories = NULL; 
    	int dir_count = 0;       	
	
	int file_count = 0;
	int total_size = 0;
	
	int max_size = 0;
	char *max_file;
		
	struct dirent *d;
	
	while ((d = readdir(dir)) != NULL){
		if (d->d_type != DT_DIR){
		       	++file_count;
		        int file_size = fsize(concat(concat(path, "/"), d -> d_name));
		       	if (file_size > max_size){
		       		max_size = file_size;
		       		max_file = d -> d_name;
		       	};
		       	total_size += file_size;
		} else {
			char *dir_name = d -> d_name;
			if(strcmp(dir_name, ".") == 0 || strcmp(dir_name, "..") == 0) continue;
			dir_count++;
       			directories = (char **)realloc(directories, dir_count * sizeof(char *));
       			
       			if (directories == NULL) {
            			printf("Memory alloc error!\n");
            			exit(EXIT_FAILURE);
        		}

			directories[dir_count - 1] = concat(concat(path, "/"),  dir_name);
		};
	};
	
	
	char *offset = allocate_offset('\t', level);

	size_t needed = snprintf(NULL, 0, "%sDirectory: %s\n%s- Files count: %d\n%s- Files total size: %d\n%s- Max size file: %s\n\n", offset, path, offset,file_count, offset, total_size, offset, max_file);
    	char  *buffer = malloc(needed);
    	sprintf(buffer, "%sDirectory: %s\n%s- Files count: %d\n%s- Files total size: %d\n%s- Max size file: %s\n\n", offset, path, offset,file_count, offset, total_size, offset, max_file);
	result = buffer;
	
	if (closedir(dir) == -1) {
    		perror("Directory close failed!");
    		exit(errno);
	}
	
	for (int i = 0; i < dir_count; i++){
		result = concat(result,	printFiles(directories[i], level + 1));
	};
	
	free(offset);
	free(directories);

	return result;
	
};


int main(int argc, char *argv[]){
	if (argc != 3){
		printf("Using: <target directory path> <output file path>\n");
		return 1;
	};
	
	char* result = printFiles(argv[1], 0);	
	
	printf("%s", result);
	writeToFile(argv[2], result);

	return 0;
};
