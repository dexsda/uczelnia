#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

char str[256];
int handle;
pthread_t * thread_ids;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
int end;

struct thread_params {
	int id;
	int total;
	int buflen;
};

void cancel_threads(int k, pthread_t self){
	int i;
	for(i=0;i<k;i++)
		if(thread_ids[i]!=self)
			pthread_cancel(thread_ids[i]);
}

void kill_info(int signo)
{
	printf("sygnal %d %ld\n",getpid(),pthread_self());
	//exit(0);
}

void * sigfpe(void * params)
{
	int a=0;
	int b=7;
	int c=b/a;
	printf("%d\n",c);
	return NULL;
}


void * strloc(void * params)
{
#ifdef SIG3
	signal(SIGUSR1,kill_info);
	signal(SIGTERM,kill_info);
	signal(SIGKILL,kill_info);
	signal(SIGSTOP,kill_info);
#endif
#ifdef SIG4
	signal(SIGUSR1,SIG_IGN);
	signal(SIGTERM,SIG_IGN);
	signal(SIGKILL,SIG_IGN);
	signal(SIGSTOP,SIG_IGN);
#endif
#ifdef SIG5
	signal(SIGUSR1,kill_info);
	signal(SIGTERM,kill_info);
	signal(SIGKILL,kill_info);
	signal(SIGSTOP,kill_info);
#endif
	struct thread_params * args = (struct thread_params *) params;
	char * buf=malloc(args->buflen);
#ifdef VER1
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
#elif VER2
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
#elif VER3
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
#endif
	int chunk=0,offset,bytes=1,i,ptr,curoffset;
	while (1) {
		offset=(chunk*args->total + args->id)*args->buflen;
		pthread_mutex_lock(&mutex);
		lseek(handle,offset,SEEK_SET);
		bytes=read(handle,buf,args->buflen);
		pthread_mutex_unlock(&mutex);
		if(bytes==0 || offset>end)
			break;
		//printf("%d: %d\n",(int)pthread_self(),offset);
		curoffset=offset;
		ptr=0;
		for(i=0; i<bytes; i++){
			if(ptr==strlen(str)){
				printf("Watek %ld znalazl '%s' (%d)\n",pthread_self(),str,curoffset);
#ifndef VER3
				if(pthread_mutex_trylock(&mutex2)!=EBUSY){
					cancel_threads(args->total,pthread_self());
					pthread_exit(NULL);
				}
#endif
				curoffset+=ptr;
				ptr=0;
			}
			if(buf[i]==str[ptr]) {
				ptr++;
			} else {
				curoffset+=ptr;
				ptr=0;
				curoffset++;
			}
		}
		if(ptr==strlen(str)){
			printf("Watek %d znalazl '%s' (%d)\n",(int)pthread_self(),str,curoffset);
#ifndef VER3
			if(pthread_mutex_trylock(&mutex2)!=EBUSY){
				cancel_threads(args->total,pthread_self());
				pthread_exit(NULL);
			}
#endif
			ptr=0;
		}
		while(ptr>0){
			offset+=args->buflen;
			pthread_mutex_lock(&mutex);
			lseek(handle,offset,SEEK_SET);
			bytes=read(handle,buf,args->buflen);
			pthread_mutex_unlock(&mutex);
			for(i=0; i<bytes; i++){
				if(ptr==strlen(str)){
					printf("Watek %d znalazl '%s' (%d)\n",(int)pthread_self(),str,curoffset);
#ifndef VER3
					if(pthread_mutex_trylock(&mutex2)!=EBUSY){
						cancel_threads(args->total,pthread_self());
						pthread_exit(NULL);
					}
#endif
					ptr=0;
					break;
				} else if (buf[i]!=str[ptr]) {
					ptr=0;
					break;
				} else {
					ptr++;
				}
			}
		}
		chunk++;
#ifdef VER2
		pthread_testcancel();
#endif
	}
	free(buf);
	return NULL;
}

int main(int argc, char * argv[])
{
	if(argc!=5){
		fprintf(stderr,"%s [watki] [nazwa] [bufor] [napis]\n",argv[0]);
		exit(1);
	}
#ifdef SIG1
	signal(SIGUSR1,SIG_IGN);
	signal(SIGTERM,SIG_IGN);
	signal(SIGKILL,SIG_IGN);
	signal(SIGSTOP,SIG_IGN);
#endif
#ifdef SIG2
	signal(SIGUSR1,kill_info);
	signal(SIGTERM,kill_info);
	signal(SIGKILL,kill_info);
	signal(SIGSTOP,kill_info);
#endif
#ifdef SIG3
	signal(SIGUSR1,kill_info);
	signal(SIGTERM,kill_info);
	signal(SIGKILL,kill_info);
	signal(SIGSTOP,kill_info);
#endif
	pthread_attr_t * attr=malloc(sizeof(pthread_attr_t));
	int threads=atoi(argv[1]);
	if((handle=open(argv[2],O_RDONLY))<0)
		perror("blad odczytu");
	end=lseek(handle,0,SEEK_END);
	int buflen=atoi(argv[3]);
	strcpy(str,argv[4]);
	thread_ids = malloc(sizeof(pthread_t)*threads);
	struct thread_params * thread_args = malloc(sizeof(struct thread_params)*threads);
	int i;
#ifdef SIG6
	pthread_create(&(thread_ids[0]),NULL,&sigfpe,NULL);
	for(i=0; i<10; i++)
		printf("another thread\n");
	pthread_join(thread_ids[0],NULL);
	return 0;
#endif
#ifdef VER3
	pthread_attr_setdetachstate(attr, PTHREAD_CREATE_DETACHED);
#else
	attr = NULL;
#endif
	for(i=0; i<threads; i++){
		thread_args[i].id=i;
		thread_args[i].total=threads;
		thread_args[i].buflen=buflen;
		pthread_create (&(thread_ids[i]),attr, &strloc, &(thread_args[i]));
	}
#ifndef SIG4
#ifndef SIG5
//	sleep(1);
	kill(getpid(),SIGUSR1);
//	kill(getpid(),SIGTERM);
//	kill(getpid(),SIGKILL);
//	kill(getpid(),SIGSTOP);
#endif
#endif
#ifdef SIG4
	for(i=0; i<threads; i++)
		pthread_kill(thread_ids[i],SIGUSR1);
#endif
#ifdef SIG5
	for(i=0; i<threads; i++)
		pthread_kill(thread_ids[i],SIGUSR1);
#endif
	for(i=0; i<threads; i++)
#ifndef VER3
		pthread_join(thread_ids[i],NULL);
#else
		pthread_detach(thread_ids[i]);
#endif
	close(handle);
	pthread_exit(NULL);
	return 0;
}
