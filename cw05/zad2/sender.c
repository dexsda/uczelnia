#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define KILL 0
#define QUEUE 1

void send(int mode, int value){
	pid_t pid;
	FILE * handle = popen("pidof receiver","r");
	if(fscanf(handle,"%d",&pid) == EOF){
		fprintf(stderr,"receiver musi dzialac\n");
		pclose(handle);
		_exit(1);
	}
	pclose(handle);
	union sigval val;
	val.sival_int=value;
	mode==KILL ? kill(pid,SIGUSR1) : sigqueue(pid,SIGUSR1,val);
}

int main(int argc, char * argv[])
{
	if(argc==2 && !strcmp(argv[1],"kill")){
		send(KILL,0);
	} else if (argc==3 && !strcmp(argv[1],"sigqueue")){
		int val=atoi(argv[2]);
		send(QUEUE,val);
	} else {
		fprintf(stderr,"Blad wywolania! sender <kill|sigqueue <val>>\n");
	}
	return 0;
}
