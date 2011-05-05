#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int sigcount = 0;

void usr2_handler(int signo,siginfo_t *info,void *unused){
	printf("Otrzymano %d sygnalow\n",sigcount);
	_exit(-1);
}

void usr1_handler(int signo,siginfo_t *info,void *unused){
	++sigcount;
}

int main(void){
	struct sigaction act1,act2;
	act1.sa_flags = 0;
	sigfillset(&act1.sa_mask);
	act1.sa_sigaction = usr1_handler;
	act2.sa_flags = 0;
	sigfillset(&act2.sa_mask);
	act2.sa_sigaction = usr2_handler;
	sigaction(SIGUSR1,&act1,NULL);
	sigaction(SIGUSR2,&act2,NULL);
	for(;;);
	exit(0);
}

