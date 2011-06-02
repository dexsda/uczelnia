#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "header.h"

sem_t* sem;
int *mem;

void int_handler(){
	if(mem!=MAP_FAILED)
		munmap(mem,200);
	exit(0);
}

int main(int argc, char** argv)
{
	int operacja, shm_id,i;
	int res;
	if(argc != 2)
	{
		printf("Podaj jaka operacje ma przeprowadzac czytelnik![MIN | MAX | AVG]\n");
		return -1;
	}
	signal(SIGINT, int_handler);
	if(!strcmp(argv[1], "min")) {
		operacja = 0;
	} else if(!strcmp(argv[1],"max")) {
		operacja = 1;
	} else if(!strcmp(argv[1], "avg")) {
		operacja = 2;
	} else	{
		printf("Podaj operacje [min|max|avg]\n");
		return -1;
	}
	
	if((shm_id = shm_open("/cw08shm", O_RDWR, 0))< 0){
		perror("shm_open error");
		return -1;
	}
	
	if((sem = sem_open("/cw08sem",  0))==SEM_FAILED){
		perror("sem_open error");
		return -1;
	}

	if (ftruncate(shm_id, sizeof(mem)) <0) {
		perror("ftruncate error");
		return -1;
	}
	
	if((mem = (int*)mmap(NULL, 200*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0))<0){
		perror("mmap error");
		return -1;
	}
	
	while(1){ 
		if(sem_wait(sem)<0) {
			perror("sem_wait error");
			return -1;
		}
		
		if(operacja==0) {
			res=mem[0];
			for(i=0;i<200;i++)
				if(res>mem[i])
					res=mem[i];
			printf("min: %d\n",res);
		} else if(operacja==1) {
			res=mem[0];
			for(i=0;i<200;i++)
				if(res<mem[i])
					res=mem[i];
			printf("max: %d\n",res);
		} else {
			res=0;
			for(i = 0; i < 200; i++)
				res+=mem[i];
			printf("avg: %f\n",(double)res/200.0);
		}
		
		if(sem_post(sem)<0) {
			perror("sem_post error");
			return -1;
		}
	}
	
	return 0;
}



