#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

char string[128];

void reverse_string(){
	int i,j,c;
	for(i=0,j=strlen(string)-1;i<j;i++,j--){
		c=string[i];
		string[i]=string[j];
		string[j]=c;
	}
}

void tstp_handler(int sig){
	reverse_string();
}

void int_handler(int sig){
	puts("Odebrano sygnal SIGINT");
	exit(1);
}

int main(int argc, char * argv[]){
	if(argc<2){
		fprintf(stderr,"podaj argument stringa do wypisania");
		exit(0);
	} else {
		strcpy(string,argv[1]);
	}
	struct sigaction act;
	sigset_t set;
	sigemptyset(&set);
	act.sa_handler=int_handler;
	act.sa_mask=set;
	act.sa_flags=0;
	sigaction(SIGINT,&act,NULL);
	signal(SIGTSTP, tstp_handler);
	while(1){
		puts(string);
		sleep(1);
	}
	return 0;
}
