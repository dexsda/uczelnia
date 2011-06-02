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

sem_t* sem,*sem2;
int *mem;

void int_handler(int signo){
	if(shm_unlink("/cw08shm")<0)
		perror("shm_unlink error");
	if(sem_unlink("/cw08sem")<0)
		perror("sem_unlink error");
	if(sem_unlink("/cw08sem2")<0)
		perror("sem_unlink error");
	if(mem!=MAP_FAILED)
		munmap(mem,200);
	exit(0);
}

int main() {
	int shm_id;
	signal(SIGINT, int_handler);
	if((shm_id = shm_open("/cw08shm", O_RDWR | O_CREAT, 0700)) <0) {
		perror("shm_open error");
		return -1;
	}

	if((sem = sem_open("/cw08sem", O_CREAT, 0700, SALA))==SEM_FAILED) {
		perror("sem_open error");
		return -1;
	}

	if((sem2 = sem_open("/cw08sem2", O_CREAT | O_EXCL, 0700, 1))==SEM_FAILED){
		perror("sem_open error");
		return -1;
	}
	if (ftruncate(shm_id, sizeof(mem)) <0) {
		perror("ftruncate error");
		return -1;
	}
	if((mem = (int*)mmap(NULL, 200*sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shm_id, 0))<0){
		perror("mmap error");
		return -1;
	}
	
	int i;
	for(i=0;i<200;i++)
		mem[i]=rand()%100-50;
	while(1);
	return 0;
}
