#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); 
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int copyFile(const char *src_path, const char *dst_path) {
	int total_bytes = 0;
	
	FILE *src = fopen(src_path, "r");
	if (src == NULL) {
		perror(src_path);
		exit(errno);
	}

	FILE *dst = fopen(dst_path, "w");
	if (dst == NULL) {
		perror(dst_path);
		exit(errno);
	}
	
	char c = '\0';
	while (1)  {
		c = fgetc(src);
		if (ferror(src)) { 
        		perror("File read error!\n"); 
        		exit(errno);
        	}
        	  
		if (c == EOF) break;
		fputc(c, dst);
		if (ferror(src)) { 
        		perror("File write error!\n"); 
        		exit(errno);
        	}
	}

	fclose(src);
	fclose(dst);
	
	struct stat src_stat;
	stat(src_path, &src_stat);
	chmod(dst_path, src_stat.st_mode);
	
	return src_stat.st_size;
}


int main(int argc, char *argv[]) {
	if (argc != 4){
		printf("Usage: <source directory> <destination directory> <max processes>\n");
		return 1;
	}

	DIR *src_dir = opendir(argv[1]);
	if (src_dir == NULL){
		perror("Source directory open failed!");
		exit(errno);
	}
	
	
	int max_proc = atoi(argv[3]);
	
	int runningProcesses = 0;
	struct dirent *d;

	while ((d = readdir(src_dir)) != NULL){
		if (d->d_type != DT_DIR){
		
			if (runningProcesses >= max_proc) {
			    wait(NULL);
			    runningProcesses--;
			}
			
			int pid = fork();
			if (pid == -1) {
			    perror("Fork_error!");
			    exit(EXIT_FAILURE);
			} else if (pid == 0) { 
			    printf("PID: %d, Start copying file: %s\n", getpid(), d->d_name);
			    
			    int bytes_copyed = copyFile(concat(argv[1], concat("/", d->d_name)), concat(argv[2], concat("/", d->d_name)));
			    
			    printf("PID: %d, Copying is completed, %d bytes have been copied\n", getpid(), bytes_copyed);
			   
			    exit(EXIT_SUCCESS);
			}

			runningProcesses++;
			
		}
	}
	
	
	
	if (closedir(src_dir) == -1) {
    		perror("Source directory close failed!");
    		exit(errno);
	}
	
	
	if (system("ps -x | grep \"t2\" | grep -v \"grep\"") != 0) {
        	perror("System call error!");
        	exit(errno);
    	}
	while (wait(NULL) > 0);
	
	return 0;
}





























