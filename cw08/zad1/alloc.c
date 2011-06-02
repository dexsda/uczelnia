#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "header.h"

int mem_id,sem_id;

void int_handler(int signo){	
	if(((semctl(sem_id, IPC_RMID, 0)) == -1)) //jezeli semafor jest stworzony przez nas, usun
		perror("semctl error");
	if(((shmctl(mem_id, IPC_RMID, 0)) == -1)) //jezeli pamiec jest stworzona przez nas, usun
		perror("shmctl error");
	exit(0);
}

int main () {
	signal(SIGINT,int_handler);
	if((mem_id=shmget(ftok(".", 32),NALL*sizeof(struct msg)+2*sizeof(int), IPC_CREAT | 0777))<0){
		perror("shmget error");
		return -1;
	}
	if((sem_id=semget(ftok(".",33),3,IPC_CREAT | IPC_EXCL | 0777))<0)
	{
		perror("semget error");
		return -1;	
	} else {
		printf("semafory\n");
		if(semctl(sem_id,0,SETVAL,0)<0) { //konsumenci
			perror("semctl error");
			return -1;
		}
		if(semctl(sem_id,1,SETVAL,1)<0) { //mutex
			perror("semctl error");
			return -1;
		}
		if(semctl(sem_id,2,SETVAL,NALL)<0) { //producenci
			perror("semctl error");
			return -1;
		}
	}
	while(1);
	return 0;
}
