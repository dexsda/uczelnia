#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int d;
int res=1;
void fib_1(int a)
{
	if(--d)
		raise(SIGUSR2);
}
void fib_2(int a)
{
	res*=d;
	raise(SIGUSR1);
}

int main(int argc, char * argv[])
{
	d=4;
	if(argc>1)
		d=atoi(argv[1]);
	signal(SIGUSR1,fib_1);
	signal(SIGUSR2,fib_2);
	raise(SIGUSR2);
	printf("%d\n",res);
	return 0;
}
