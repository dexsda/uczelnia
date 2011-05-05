#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	int i,status;
	pid_t pid;
	if(argc != 2){
		fprintf(stderr,"uzycie: ./parent [signals]\n");
		exit(0);
	}
	i = atoi(argv[1]);
	if((pid=fork())){
		sleep(1);//aby pozwolic procesowi potomnemu zainicjalizowac wszystko
		while(i--){
			//sleep(1);
			kill(pid,SIGUSR1);
		}
		kill(pid,SIGUSR2);
	} else {
		execlp("./child","./child",NULL);
	}
	wait(&status);
	return 0;
}
