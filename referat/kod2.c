#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int signal){
	printf("Ignoruje sygnal %d",signal);
}

int main(void)
{
	int i = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, handler);
	//signal(SIGKILL, SIG_IGN);
	while(1){
		printf("i = %d\n", i);
		sleep(1);
		i++;
	}
	return 0;
}
