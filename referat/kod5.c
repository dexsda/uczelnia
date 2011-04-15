#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void alrm_handler(int sig){
	puts("Koniec blokady");
}

int main(void)
{
	sigset_t set,oldset;
	sigemptyset(&set);
	sigaddset(&set,SIGINT);
	sigprocmask(SIG_BLOCK, &set, &oldset);
	signal(SIGALRM,alrm_handler);
	alarm(10);
	printf("Oczekuje na niezablokowany sygnal\n");
	pause();
	sigprocmask(SIG_SETMASK, &oldset, NULL);
	while(1){
		printf("SIGINT juz niezablokowany\n");
		sleep(1);
	}
	return 0;
}
