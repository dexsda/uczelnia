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
#include <time.h>
#include "header.h"

sem_t* sem;
sem_t* sem2;
int *mem;


void int_handler(){
	if(mem!=MAP_FAILED)
		munmap(mem,200);
	exit(0);
}

int main(int argc, char** argv)
{
	int shm_id,i;
	signal(SIGINT, int_handler);
	if((shm_id = shm_open("/cw08shm", O_RDWR, 0))< 0) {
		perror("shm_open error");
		return -1;
	}
	if((sem = sem_open("/cw08sem",  0))==SEM_FAILED) {
		perror("sem_open error");
		return -1;
	}
	if((sem2 = sem_open("/cw08sem2",  0)) ==SEM_FAILED) {
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
	
	while(1){
		if(sem_wait(sem2)<0) {
			perror("sem_wait error");
			return -1;
		}
		printf("Pisarzy nie ma - oczekiwanie na czytelnikow\n");
		for(i=0;i<SALA;i++){
			if(sem_wait(sem)<0){
				perror("sem_wait error");
				return -1;
			}
		}

		mem[rand()%200]=rand()%100-50;
		printf("Zmieniono wartosc w tablicy\n");
		for(i=0;i<SALA;i++){
			if(sem_post(sem)<0){
				perror("sem_wait error");
				return -1;
			}
		}
		if(sem_post(sem2)<0){
			perror("sem_post error");
			return -1;
		}
		sleep(1);
	}
	return 0;
}

