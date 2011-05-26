#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main()
{
	pid_t pid = getpid();
	printf("%d ",pid);
	int boolean;
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	int list[20];
	int i,j;
	for(;;){
		i=0;
		boolean=0;
		while(scanf("%d",list+i)==1)
			if(list[i++] == pid)
				boolean=1;
		if(boolean){
			fprintf(stderr,"\nproc %d: ",pid);
			for(j=0;j<i; j++)
				fprintf(stderr,"%d ",list[j]);
		} else {
			for(j=0;j<i; j++)
				printf("%d ",list[j]);
			printf("%d ",pid);
		}
		usleep(5000);
	}
	return 0;
}
