#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int block=1;
void alrm_handler(int sig){
	puts("Koniec blokady obslugi");
	block=0;
	sigset_t set,oldset;
	sigemptyset(&set);
	sigaddset(&set,SIGINT);
	sigprocmask(SIG_UNBLOCK, &set, &oldset);
}

int main(void)
{
	int boolean=1;
	sigset_t set,oldset;
	sigemptyset(&set);
	sigaddset(&set,SIGINT);
	sigprocmask(SIG_BLOCK, &set, &oldset);
	signal(SIGALRM,alrm_handler);
	while(1){
		sleep(1);
		sigpending(&set);
		if(sigismember(&set,SIGINT)){
			printf("SIGINT oczekuje na obsluge\n");
			if(boolean){
				alarm(5);
				boolean=0;
			}
		}
	}
	return 0;
}
