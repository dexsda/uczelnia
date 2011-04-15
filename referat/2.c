#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int b=0;
void handler_sigsegv(int a)
{
	b=1;
	psignal(a,"dzielenie przez zero!");
}

int main(int argc, char * argv[])
{
	signal(SIGFPE,handler_sigsegv);
	int a=37/b;
	printf("Doszedlem tutaj!\n");
	return 0;
}
