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
void * mem;

void int_handler(int signo) {
	if(shmdt(mem)<0)
		perror("Blad shmdt");
	exit(0);
}

int main (int argc, char * argv[]) {
	int i,j,k;
	int * ptr_write;
	struct msg * messages;
	struct sembuf lower[2], raise[2];
	struct msg temp;
	signal(SIGINT,int_handler);
	srand(time(NULL));
	if((mem_id=shmget(ftok(".",32),NALL*sizeof(struct msg)+2*sizeof(int),0))<0) {
		perror("shmget error");
		return -1;
	}
	if(!(mem=shmat(mem_id,NULL,0))) {
		perror("shmat error");
		return -1;
	}
	if((sem_id=semget(ftok(".",33),3,0))<0) {
		perror("Blad semget");
		return -1;
	}
	ptr_write=(int*)mem;
	messages=(struct msg*)(mem+2*sizeof(int));
	lower[0].sem_num=2;
	lower[1].sem_num=1;
	lower[0].sem_op=-1;
	lower[1].sem_op=-1;
	raise[0].sem_num=0;
	raise[1].sem_num=1;
	raise[0].sem_op=1;
	raise[1].sem_op=1;
	lower[0].sem_flg=lower[1].sem_flg=raise[0].sem_flg=raise[1].sem_flg=0;
	while(1)
	{
		if(semop(sem_id,lower,2)<0) { //opusc semafor producenta i licznikowy
			perror("semop error");
			return -1;
		}
		k=*ptr_write;
		(*ptr_write)=(k+1)%NALL;
		temp.op=rand()%3;
		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				temp.mat1[i][j]=rand()%42;
				temp.mat2[i][j]=rand()%42;
			}
		}
		messages[k]=temp;
		printf("Zadanie umieszczone w pamieci (typ %d)\n",temp.op);
		if(semop(sem_id,raise,2)<0)
		{
			perror("Blad semop");
			return -1;
		}
		sleep(1);
	}
	return 0;
}
