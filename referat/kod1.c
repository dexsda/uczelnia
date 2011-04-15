#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(void)
{
	int i = 0;
	alarm(3);
	while (1)
	{
		printf("i = %d\n", i);
		sleep(1);
		i++;
	}
	return 0;
}
