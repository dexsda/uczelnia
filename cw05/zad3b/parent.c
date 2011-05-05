#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int start=0;
void usr1_handler(int signo,siginfo_t *info,void *unused){
	if(start++==0)
		puts("rozpoczeto wysylanie");
}

int main(int argc, char *argv[]){
	int i,status;
	struct sigaction act;
	act.sa_flags = 0;
	sigfillset(&act.sa_mask);
	act.sa_sigaction = usr1_handler;
	sigaction(SIGUSR1,&act,NULL);
	sigset_t mask;
	pid_t pid;
	if(argc != 2){
		fprintf(stderr,"uzycie: ./parent [signals]\n");
		exit(0);
	}
	i = atoi(argv[1]);
	sigemptyset(&mask);
	sigaddset(&mask,SIGUSR1);
	sigprocmask(SIG_BLOCK,&mask,NULL);
	sigfillset(&mask);
	sigdelset(&mask,SIGUSR1);
	if((pid=fork())){
		sigsuspend(&mask);
		while(i--){
			//sleep(1);
			kill(pid,SIGUSR1);
			sigsuspend(&mask);
		}
		kill(pid,SIGUSR2);
	} else {
		execlp("./child","./child",NULL);
	}
	wait(&status);
	return 0;
}
