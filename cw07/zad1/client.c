#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/msg.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "header.h"

int id;
struct msg_client msg_send;


// funkcja generujaca komunikat do wyslania
void fillmsg(){
	msg_send.id=id;
	int i;
	int n=rand()%MAX;
	for (i=0;i<=n;i++)
		msg_send.msg[i]='A'+rand()%26;
	for (n++;n<MAX;n++)
		msg_send.msg[n]=0;
}

struct msg_server msg_rec;

// funkcja wykonywana przy przechwyceniu syganlu SIGUSR1

void handler_int(int n){
	if(msgctl(id, IPC_RMID, 0) == -1){
		perror("Blad dzialania funkcji msgctl\n");
		exit(1);
	}
	exit(0);
}

int main(int argc, char ** argv){
	if(argc != 2){
		fprintf(stderr, "Skladnia wywolania: ./client <nazwa>\n");
		exit(1);
	}
	if(strlen(argv[1]) >= 21){
		fprintf(stderr, "Dlugosc nazwy to max 20\n");
		exit(1);
	}
	int number = 0;
	int idserv;
	srand(time(NULL));
	if((id = msgget(ftok("client", getpid()), IPC_CREAT | 0600)) == -1){
		perror("Blad msgget");
		exit(1);
	}
	if((idserv = msgget(ftok(".", 32), 0600)) == -1){
		perror("Blad msgget - kolejka serwera nie uruchomiona");
		exit(1);
	}
	if(signal(SIGINT, handler_int) == SIG_ERR){
		perror("Blad dzialania funkcji signal");
		exit(1);
	}
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	fprintf(stdout, "Ile wyslac komunikatow?\n");
	strcpy(msg_send.name,argv[1]);
	while(1){
		if(fscanf(stdin, "%d", &number)!=EOF){
			while(number--){
				fillmsg();
				int i;
				msg_send.type = number+1;
				if(msgsnd(idserv, &msg_send, sizeof(msg_send)-sizeof(long), 0) == -1){
					perror("Blad dzialania funkcji msg_send\n");
					exit(1);
				}
				fprintf(stdout, "Wyslalem: %s\n\n", msg_send.msg);
			}
		}
		if (msgrcv(id, &msg_rec, sizeof(msg_rec)-sizeof(long), 1, IPC_NOWAIT) == -1) {
		} else {
			fprintf(stdout, "Otrzymano wiadomosc zwrotna od serwera (jestem klientem %s)\n%s\n", msg_rec.name,msg_rec.msg);
			fprintf(stdout, "\tkomunikatow: %d\n\tbajtow: %d\n\tmax bajtow: %d\n", (int)msg_rec.msgs, (int)msg_rec.cbytes, (int)msg_rec.qbytes);
			fprintf(stdout, "\tpid odczytu: %d\n\tpid zapisu: %d\n", (int)msg_rec.rpid, (int)msg_rec.wpid);
			fprintf(stdout, "\tczas odczytu: %s\tczas zapisu: %s\n", ctime(&msg_rec.rtime), ctime(&msg_rec.wtime));
		}
	}
	return 0;
}
