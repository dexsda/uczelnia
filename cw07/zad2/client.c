#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <mqueue.h>
#include "header.h"

struct msg_cli msgs;
struct msg_serv msgr;
mqd_t KS;
mqd_t KK;
char queue[12]; //nazwa

void finish(){
	mq_close(KK);
	mq_close(KS);
	exit(0);
}

void handler_init(int signo){
	finish();
}

void handler_usr1(int signo){
	struct mq_attr attr;
	while(1){
		if(mq_getattr(KK, &attr) == -1){
			perror("blad dzialania mq_getattr KK");
			finish();
		}
		if(!attr.mq_curmsgs)
			break;
		if(mq_receive(KK, (char *)&msgr, sizeof(msgr), 0) == -1){
			perror("Blad dzialania mq_receive");
			finish();
		}
		fprintf(stdout, "Odebralem wiadomosc zwrotna: \t%s\nklient: \t%s\nls: \t%d\nliczba juz wyslanych komunikatow: %d\n\n", msgr.message, msgr.name, msgr.ls, msgr.number);
	}
}

void fillmsg(){
	strcpy(msgs.name, queue);
	msgs.pid=getpid();
	int i;
	int n=rand()%MAX;
	for (i=0;i<=n;i++)
		msgs.message[i]='A'+rand()%26;
	for (n++;n<MAX;n++)
		msgs.message[n]=0;
}

int main(int argc, char ** argv){
	if(argc != 2){
		fprintf(stderr, "./client <nazwa>\n");
		exit(1);
	}
	if(strlen(argv[1]) > 12){
		fprintf(stderr, "Dlugosc nazwy to max 12\n");
		exit(1);
	}
	int i;
	strcpy(queue,argv[1]);
	srand(time(NULL));
	if(signal(SIGINT, finish) == SIG_ERR){
		perror("Blad ustalania handlera SIGINT");
		finish();
	}
	if(signal(SIGUSR1, handler_usr1) == SIG_ERR){
		perror("Blad ustalania handlera SIGUSR1");
		finish();
	}
	if((KS = mq_open("/KS", O_WRONLY)) == -1){
		perror("Blad mq_open KS");
		finish();
	}
	if((KK = mq_open("/KK", O_RDONLY)) == -1){
		perror("Blad mq_open KK");
		finish();
	}
	int count = 0;
	struct mq_attr attr;
	while(1){
		fprintf(stdout, "Ile wyslac komunikatow?\n");
		scanf("%d", &count);
		while(count--){
			fillmsg();
			if(mq_getattr(KS, &attr) == -1){
				perror("Blad mq_getattr");
				finish();
			}
			if(mq_send(KS, (char *)&msgs, sizeof(msgs), 0) == -1){
				perror("Blad mq_send");
				finish();
			}
			fprintf(stdout, "Wyslalem wiadomosc: \n\t%s\n", msgs.message);
		}
	}
	return 0;
}
