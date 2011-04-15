#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void alrm_handler(int sig){
	puts("Koniec wypisywania");
	_exit(0);
}

int main(void)
{
	int i = 0;
	struct sigaction act;
	sigset_t set;
	sigemptyset(&set);
	act.sa_handler=alrm_handler;
	act.sa_mask=set;
	act.sa_flags=0;
	sigaction(SIGALRM,&act,NULL);
	alarm(5);
	while (1) {
		printf("i = %d\n", i);
		sleep(1);
		i++;
	}
	return 0;
}
