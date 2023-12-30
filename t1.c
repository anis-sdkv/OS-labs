#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <time.h>
#include <sys/time.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


void printTime() {
 	struct timeval tv;
	gettimeofday(&tv, NULL); 
	
	struct tm *tm_info;
    	tm_info = localtime(&tv.tv_sec);

    	printf("Time: %02d:%02d:%02d:%03ld\n", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, tv.tv_usec / 1000);
}

int main() {
	
	pid_t child1 = fork();
	
	if (child1 == -1) {
        	perror("Child 1 fork error!");
        	exit(errno);
    	} else if (child1 == 0) {
        	printf("Child 1: pid=%d, ppid=%d, ", getpid(), getppid());
        	printTime();
        	exit(EXIT_SUCCESS);
    	}
    	
    	pid_t child2 = fork();

    	if (child2 == -1) {
        	perror("Child 2 fork error!");
        	exit(errno);
    	} else if (child2 == 0) {
        	printf("Child 2: pid=%d, ppid=%d, ", getpid(), getppid());
        	printTime();
        	exit(EXIT_SUCCESS);
    	}

	printf("Parent: pid=%d, ppid=%d, ", getpid(), getppid());
    	printTime();
    	

	waitpid(child1, NULL, 0);
    	waitpid(child2, NULL, 0);

	if (system("ps -x | grep \"t1\"") != 0) {
        	perror("System call error!");
        	exit(errno);
    	}

    	
    	return 0;
}

