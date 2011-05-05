#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void usr1_handler(int signo, siginfo_t * info, void * context){
	if(info->si_code==SI_USER){
		printf("KILL [pid:%d, uid:%d, utime:%f]\n",info->si_pid,info->si_uid,(double)info->si_utime);
	} else if (info->si_code==SI_QUEUE){
		printf("QUEUE [pid:%d, uid:%d, utime:%f]: %d\n",info->si_pid,info->si_uid,(double)info->si_utime,(int)info->si_value.sival_int);
	}
}

int main()
{
	struct sigaction act;
	act.sa_flags=SA_SIGINFO;
	sigfillset(&act.sa_mask);
	act.sa_sigaction=usr1_handler;
	sigaction(SIGUSR1,&act,NULL);
	while (1);
	return 0;
}
