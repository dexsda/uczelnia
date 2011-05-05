#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int sigcount = 0;

void rtmin1_handler(int signo,siginfo_t *info,void *unused){
	printf("Otrzymano %d sygnalow\n",sigcount);
	_exit(-1);
}

void rtmin_handler(int signo,siginfo_t *info,void *unused){
	++sigcount;
}

int main(void){
	struct sigaction act1,act2;
	act1.sa_flags = 0;
	sigfillset(&act1.sa_mask);
	act1.sa_sigaction = rtmin_handler;
	act2.sa_flags = 0;
	sigfillset(&act2.sa_mask);
	act2.sa_sigaction = rtmin1_handler;
	sigaction(SIGRTMIN,&act1,NULL);
	sigaction(SIGRTMIN+1,&act2,NULL);
	union sigval val;
	val.sival_int=0;
	sigqueue(getppid(),SIGRTMIN,val);
	for(;;);
	exit(0);
}

