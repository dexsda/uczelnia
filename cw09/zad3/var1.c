#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "header.h"

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
	for(i=0; i<args->ops; i++){
		pthread_mutex_lock(&mutex_read);
		if(writer_in_da_house)
			pthread_cond_wait(&cond_read,&mutex_read);
		reader_dawgs++;
		pthread_mutex_unlock(&mutex_read);
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
	}
	return NULL;
}

void * writer_thread(void * params)
{
	struct thread_params * args = params;
	int i;
	for(i=0; i<args->ops; i++){
		pthread_mutex_lock(&mutex_write);
		if(reader_dawgs)
			pthread_cond_wait(&cond_write,&mutex_write);
		writer_in_da_house=1;
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
	}
	return NULL;
}

int main(int argc, char * argv[])
{
	if(argc!=3){
		fprintf(stderr,"%s [pisarze] [czytelnicy]\n",argv[0]);
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
	for(i=0; i<readers; i++){
		reader_args[i].id=i;
		reader_args[i].sleeptime=rand()%2 + 1;
		reader_args[i].ops=rand()%10+5;
		pthread_create (&(reader_ids[i]),NULL, &reader_thread, &(reader_args[i]));
	}

	for(i=0; i<writers; i++){
		writer_args[i].id=i;
		writer_args[i].sleeptime=rand()%2 + 1;
		writer_args[i].ops=rand()%10+5;
		pthread_create (&(writer_ids[i]),NULL, &writer_thread, &(writer_args[i]));
	}
	for(i=0; i<readers; i++)
		pthread_join(reader_ids[i],NULL);
	for(i=0; i<writers; i++)
		pthread_join(writer_ids[i],NULL);
	return 0;
}
