#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int start=0;
void rtmin_handler(int signo,siginfo_t *info,void *unused){
	if(start++==0)
		puts("rozpoczeto wysylanie");
}

int main(int argc, char *argv[]){
	int i,status;
	pid_t pid;
	struct sigaction act;
	act.sa_flags = 0;
	sigfillset(&act.sa_mask);
	act.sa_sigaction = rtmin_handler;
	sigaction(SIGRTMIN,&act,NULL);
	if(argc != 2){
		fprintf(stderr,"uzycie: ./parent [signals]\n");
		exit(0);
	}
	i = atoi(argv[1]);
	if((pid=fork())){
		sleep(1);//aby pozwolic procesowi potomnemu zainicjalizowac wszystko
		union sigval val;
		val.sival_int=0;
		while(i--){
			//sleep(1);
			sigqueue(pid,SIGRTMIN,val);
		}
		sigqueue(pid,SIGRTMIN+1,val);
	} else {
		execlp("./child","./child",NULL);
	}
	wait(&status);
	return 0;
}
