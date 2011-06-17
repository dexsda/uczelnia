#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "header.h"

time_t wrtime=0,rdtime=0;
int heap[MAX];
int elements=0;
pthread_mutex_t mutex_read = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_write = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_read=PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_write=PTHREAD_COND_INITIALIZER;
int writer_in_da_house=0;
int reader_dawgs=0;

struct thread_params {
	int id;
	int sleeptime;
	int ops;
};

void * reader_thread(void * params)
{
	struct thread_params * args = params;
	int i;
	time_t t1,t2;
	for(i=0; i<args->ops; i++){
		t1=time(NULL);
		pthread_mutex_lock(&mutex_read);
		if(writer_in_da_house)
			pthread_cond_wait(&cond_read,&mutex_read);
		reader_dawgs++;
		pthread_mutex_unlock(&mutex_read);
		t2=time(NULL);
		if(retmax(heap,elements)<0){
			printf("reader %d: co to za biblioteka bez ksiazek?!!?\n",args->id);
		} else {
			printf("reader %d: bardzo interesujace to '%d'!\n",args->id,retmax(heap,elements));
		}
		sleep(args->sleeptime);
		pthread_mutex_lock(&mutex_read);
		reader_dawgs--;
		if(!reader_dawgs)
			pthread_cond_broadcast(&cond_write);
		pthread_mutex_unlock(&mutex_read);
		rdtime+=t2-t1;
	}
	return NULL;
}

void * writer_thread(void * params)
{
	struct thread_params * args = params;
	int i;
	time_t t1,t2;
	for(i=0; i<args->ops; i++){
		t1=time(NULL);
		pthread_mutex_lock(&mutex_write);
		if(reader_dawgs)
			pthread_cond_wait(&cond_write,&mutex_write);
		writer_in_da_house=1;
		t2=time(NULL);
		if(rand()%3){
			int added=rand()%30 +1;
			if(add(heap,elements,added)<0){
				printf("writer %d: 'nie potrzebujemy takiej literatury', jasne...\n",args->id);
			} else {
				elements++;
				printf("writer %d: hura, wydalem ksiazke '%d'!\n",args->id,added);
			}
		} else {
			int deleted=delmax(heap, elements);
			if(deleted<0) {
				printf("writer %d: biblioteka bez ksiazek? Kiedys musze sam cos sklecic...\n",args->id);
			} else {
				elements--;
				printf("writer %d: '%d' wywalone; mierne wierszoklectwo!\n",args->id,deleted);
			}
		}
		sleep(args->sleeptime);
		writer_in_da_house=0;
		pthread_cond_broadcast(&cond_read);
		pthread_mutex_unlock(&mutex_write);
		wrtime+=t2-t1;
	}
	return NULL;
}

int main(int argc, char * argv[])
{
	if(argc!=7){
		fprintf(stderr,"%s [czyt] [pis] [czyt_op] [czyt_sl] [pis_op] [pis_sl]\n",argv[0]);
		exit(1);
	}
	int readers=atoi(argv[1]);
	int writers=atoi(argv[2]);
	pthread_t * reader_ids = malloc(sizeof(pthread_t)*readers);
	pthread_t * writer_ids = malloc(sizeof(pthread_t)*writers);
	struct thread_params * reader_args = malloc(sizeof(struct thread_params)*readers);
	struct thread_params * writer_args = malloc(sizeof(struct thread_params)*writers);
	int i;
	srand(time(NULL));
	int rops=atoi(argv[3]);
	int rslp=atoi(argv[4]);
	int wops=atoi(argv[5]);
	int wslp=atoi(argv[6]);
	for(i=0; i<readers; i++){
		reader_args[i].id=i;
		reader_args[i].sleeptime=rslp;
		reader_args[i].ops=rops;
		pthread_create (&(reader_ids[i]),NULL, &reader_thread, &(reader_args[i]));
	}

	for(i=0; i<writers; i++){
		writer_args[i].id=i;
		writer_args[i].sleeptime=wslp;
		writer_args[i].ops=wops;
		pthread_create (&(writer_ids[i]),NULL, &writer_thread, &(writer_args[i]));
	}
	for(i=0; i<readers; i++)
		pthread_join(reader_ids[i],NULL);
	for(i=0; i<writers; i++)
		pthread_join(writer_ids[i],NULL);
	printf("Przecietny czas oczekiwania na odczyt: %f \n",rdtime/(double)(rops*readers));
	printf("Przecietny czas oczekiwania na zapis: %f \n",wrtime/(double)(wops*writers));
	return 0;
}
