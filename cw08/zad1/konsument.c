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


FILE * handle;
void * mem;	
int mem_id,sem_id;
void int_handler(int signo) {
	if(shmdt(mem)<0)
		perror("Blad shmdt");
	exit(0);
}

void print_matrix(int mat[N][N]){
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++)
			fprintf(handle, "%d ", mat[i][j]);
		fprintf(handle, "\n");
	}
}

void do_op(struct msg p)
{
	int i,j,k;
	int res[N][N];
	print_matrix(p.mat1);
	if(p.op==0)
	{
		fprintf(handle, "+\n");
		for(i = 0 ; i < N ; i++)
			for(j = 0 ; j < N ; j++)
				res[i][j] = p.mat1[i][j] + p.mat2[i][j];
	} else if(p.op==1) {
		fprintf(handle, "-\n");
		for(i = 0 ; i < N ; i++)
			for(j = 0 ; j < N ; j++)
				res[i][j] = p.mat1[i][j] - p.mat2[i][j];
	} else if(p.op==2) {
		fprintf(handle, "\t*\n");
		for(i = 0 ; i < N ; i++){
			for(j = 0 ; j < N ; j++) {
				res[i][j] = 0;
				for(k = 0 ; k < N ; k++)
					res[i][j] = res[i][j] + p.mat1[i][k] * p.mat2[k][j];
			}
		}
	}
	print_matrix(p.mat2);
	fprintf(handle, "=\n");
	print_matrix(res);
	
	fprintf(handle, "\n\n\n");
}


int main (int argc, char * argv[]){
	int k;
	struct msg temp;
	int * ptr_read;
	signal(SIGINT,int_handler);
	struct msg * messages;
	struct sembuf lower[2], raise[2];
	if(argc<2) {
		printf("Podaj nazwe logu\n");
		return -1;
	}
	if(!(handle=fopen(argv[1],"w+")))
	{
		perror("fopen error");
		return -1;
	}
	srand(time(NULL));
	if((mem_id=shmget(ftok(".",32),NALL*sizeof(struct msg)+2*sizeof(int),0))<0){
		perror("shmget error");
		return -1;
	}
	if(!(mem=shmat(mem_id,NULL,0))) {
		perror("shmat error");
		return -1;
	}
	
	if((sem_id=semget(ftok(".",33),3,0))<0){
		perror("semget error");
		return -1;	
	}
	ptr_read=((int*)mem+1);
	messages=(struct msg*)(mem+2*sizeof(int));
	lower[0].sem_num=0;
	lower[1].sem_num=1;
	lower[0].sem_op=-1;
	lower[1].sem_op=-1;
	raise[0].sem_num=2;
	raise[1].sem_num=1;
	raise[0].sem_op=1;
	raise[1].sem_op=1;
	lower[0].sem_flg=lower[1].sem_flg=raise[0].sem_flg=raise[1].sem_flg=0;
	while(1)
	{
		if(semop(sem_id,lower,2)<0) { //opusc semafor konsumencki oraz mutex
			perror("semop error");
			return -1;
		}
		k=*ptr_read;
		(*ptr_read)=(k+1)%NALL;
		temp=messages[*ptr_read];
		do_op(temp);
		if(temp.op==0)
			printf("dodawanie\n");
		if(temp.op==1)
			printf("odejmowanie\n");
		if(temp.op==2)
			printf("mnozenie\n");
		if(semop(sem_id,raise,2)<0) { //podnies semafor producencki oraz mutex
			perror("semop error");
			return -1;
		}
		sleep(1);
	}
	return 0;
}
