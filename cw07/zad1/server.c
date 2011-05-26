#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "header.h"

int id;

struct msg_client msg_rec;

struct msg_server msg_send;

// koniec pracy serwera
void handler_int(int signo){
	if(msgctl(id, IPC_RMID, 0) == -1){
		perror("Blad dzialania funkcji msgctl\n");
		exit(1);
	}
	exit(0);
}

int main(int argc, char **argv){
	int length,i,order;
	if(argc != 3){
		fprintf(stdout, "./server <-n/-r> <maxsleep>.\n");
		return 0;
	}
	if(strcmp(argv[1], "-n") == 0){
		order = 0;
	} else if(strcmp(argv[1], "-r") == 0){
		order = 1;
	} else{
		fprintf(stdout, "./server <-n/-r> <maxsleep>.\n");
		return 0;
	}
	srand(time(NULL));
	int sleeplen = atoi(argv[2]);
	if((id = msgget(ftok(".", 32), IPC_CREAT | 0600)) == -1){
		perror("Blad msgget\n");
		exit(1);
	}
	signal(SIGINT, handler_int);
	struct msqid_ds stat;
	sleep(2);
	while(1){
		sleep(1+(rand()%sleeplen));
		if (!order){
			length = msgrcv(id, &msg_rec, sizeof(msg_rec)-sizeof(long), 0, IPC_NOWAIT);	//pierwsza wiadomosc w kolejce pobrana jako pierwsza
		} else{
			length = msgrcv(id, &msg_rec, sizeof(msg_rec)-sizeof(long), 1-LONG_MAX, IPC_NOWAIT);	//pobiera od typu maxpid-long_max
		}
		if(length > 0){
			fprintf(stdout, "Odebralem: %s\n", msg_rec.msg);
			if(msgctl(id, IPC_STAT, &stat) == -1){
				perror("Blad dzialania funkcji msgctl\n");
				exit(1);
			}
			msg_send.type = 1;
			msg_send.msgs = stat.msg_qnum;
			msg_send.cbytes = stat.msg_cbytes;
			msg_send.qbytes = stat.msg_qbytes;
			msg_send.rpid = stat.msg_lrpid;
			msg_send.wpid = stat.msg_lspid;
			msg_send.rtime = stat.msg_rtime;
			msg_send.wtime = stat.msg_stime;
			for(i = 0; i < NAME; i++){
				msg_send.name[i] = msg_rec.name[i];
			}
			for(i = 0; i < MAX; i++){
				msg_send.msg[i] = msg_rec.msg[i];
			}
			if(msgsnd(msg_rec.id, &msg_send, sizeof(msg_send)-sizeof(long), 0) == -1){
				perror("Blad dzialania funkcji msg_send\n");
				exit(1);
			}
			fprintf(stdout, "Wyslano wiadomosc zwrotna do klienta %s\n", msg_send.name);
			fprintf(stdout, "\tkomunikatow: %d\n\tbajtow: %d\n\tmax bajtow: %d\n", (int)msg_send.msgs, (int)msg_send.cbytes, (int)msg_send.qbytes);
			fprintf(stdout, "\tpid odczytu: %d\n\tpid zapisu: %d\n", (int)msg_send.rpid, (int)msg_send.wpid);
			fprintf(stdout, "\tczas odczytu: %s\tczas zapisu: %s\n", ctime(&msg_send.rtime), ctime(&msg_send.wtime));
		}
	}
	return 0;
}
