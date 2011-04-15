#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler_sigsegv(int a)
{
	psignal(a,"Tak nie mozna zrobic!");
}

int main(int argc, char * argv[])
{
	signal(SIGSEGV,handler_sigsegv);
	char * str = "Hello world";
	*str='h';
	printf("Doszedlem tutaj!\n");
	return 0;
}
