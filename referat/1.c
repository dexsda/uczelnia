#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(int a)
{
	printf("Dostalem sygnal %d\n",a);
}

int main(int argc, char * argv[])
{
	signal(SIGINT,handler);
	signal(SIGTERM,SIG_IGN);
	signal(SIGKILL,SIG_IGN);
	while(1){
		puts("waiting 1 sec...");
		sleep(1);
	}
	return 0;
}
