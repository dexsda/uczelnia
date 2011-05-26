#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include "header.h"

struct msg_serv msgs;
struct msg_cli msgr;
struct mq_attr attr1;
struct mq_attr attr2;
int ls = 0;
int ac = 0;
mqd_t KS;
mqd_t KK;

void finish(){
	mq_close(KK);
	mq_close(KS);
	mq_unlink("/KS");
	mq_unlink("/KK");
	exit(0);
}

void handler_int(int signo){
	finish();
}

struct sigevent sig;

void handler_usr1(int signo){
	
	if(mq_getattr(KS, &attr1) == -1){
		perror("Blad dzialania funkcji mq_getattr");
		finish();
	}
	while(attr1.mq_curmsgs){
		if(mq_receive(KS, (char *)&msgr, sizeof(msgr), 0) >= 0){
			fprintf(stdout, "Otrzymalem wiadomosc od klienta %s (%d):\n\t%s\n", msgr.name, msgr.pid, msgr.message);
			strcpy(msgs.message, msgr.message);
			msgs.number = ac;
			msgs.ls = ls;
			int j;
			for(j = 0; j < 12; j++){
				msgs.name[j] = msgr.name[j];
			}
			if(mq_send(KK, (char *)&msgs, sizeof(msgs), 0) == -1){
				perror("Blad mq_send");
				finish();
			}
			fprintf(stdout, "Wyslalem potwierdzenie do %d\n", msgr.pid);
			if(kill(msgr.pid, SIGUSR1) == -1){
				perror("Blad kill");
				finish();
			}
			ac++;
		} else {
			perror("Blad mq_receive");
			finish();
		}
		if((mq_getattr(KS, &attr1)) == -1){
			perror("Blad mq_getattr");
			finish();
		}
	}
	if(mq_notify(KS,&sig) == -1){
		perror("Blad mq_notify");
		finish();
	}
}

int main(int argc, char **argv){
	attr1.mq_maxmsg = 10;
	attr1.mq_msgsize = sizeof(struct msg_cli);
	attr2.mq_maxmsg = 10;
	attr2.mq_msgsize = sizeof(struct msg_serv);
	if((KS = mq_open("/KS", O_RDONLY | O_CREAT, 0600, &attr1)) == -1){
		perror("Blad tworzenia kolejki KS");
		exit(1);
	}
	if((KK = mq_open("/KK", O_WRONLY | O_CREAT, 0600, &attr2)) == -1){
		perror("Blad tworzenia kolejki KK");
		exit(1);
	}
	if((signal(SIGINT, finish)) == SIG_ERR){
		perror("Blad ustalania handlera SIGINT");
		finish();
	}
	if((signal(SIGUSR1, handler_usr1)) == SIG_ERR){
		perror("Blad ustalania handlera SIGUSR1");
		finish();
	}
	sig.sigev_notify=SIGEV_SIGNAL;
	sig.sigev_signo=SIGUSR1;
	if(mq_notify(KS,&sig) == -1){
		perror("Blad mq_notify");
		finish();
	}
	while(1){
		ls++;
		sleep(1);
	}
	return 0;
}
