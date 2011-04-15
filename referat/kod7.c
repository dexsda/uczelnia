#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void alrm_handler(int signal){
	puts("Alarm!");
}

int main(void)
{
	int boolean=1;
	signal(SIGALRM,alrm_handler);
	sigset_t set,oldset;
	sigemptyset(&set);
	sigaddset(&set,SIGINT);
	//sigaddset(&set,SIGALRM);
	sigsuspend(&set);
	printf("Koniec\n");
	return 0;
}
