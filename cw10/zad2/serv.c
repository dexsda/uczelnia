#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include "header.h"

struct msg message;

struct client clients[MAX_CLIENT];
size_t size_unix;
size_t size_inet;
int total,ptr;
int num;
int sunix,sinet;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void * sep_thread(void * data){
	int param=*((int*)data);
	int sock=clients[param].socket;
	while(1){
		int i,j;
		struct msg message;
		pthread_mutex_lock(&mutex);
		int recvb = recv(sock,&message,sizeof(struct msg),MSG_DONTWAIT);
		if(recvb!=-1)
			printf("%d \n",param);
		pthread_mutex_unlock(&mutex);
		if(recvb == -1 && errno!=EWOULDBLOCK ) {
			perror("recv error");
			exit(-1);
		} else if(recvb==-1) {
			//sleep(1);
		} else if(message.msg_type=='i'){
			pthread_mutex_lock(&mutex);
			i=message.cli.id;
			j=message.client_to_stat;
			if(j<ptr && clients[j].id!=-1){
				printf("odpytuje klienta %d dla klienta %d\n",j,i);
				int temp='z';
				struct info stats_temp;
				send(clients[j].socket,&temp,sizeof(int),0);
				recv(clients[j].socket,&stats_temp,sizeof(struct info),0);
				send(sock,&stats_temp,sizeof(struct info),0);
			} else {
				puts("nie ma klienta");
			}
			pthread_mutex_unlock(&mutex);
		} else if(message.msg_type=='l'){
			pthread_mutex_lock(&mutex);
			i=message.cli.id;
			printf("wysylam liste do klienta %d \n",i);
			send(sock,&total,sizeof(int),0);
			struct client * clients_temp = malloc(sizeof(struct client)*total);
			int k=0;
			for(j=0; j<ptr; j++){
				if(clients[j].id!=-1){
					clients_temp[k++] = clients[j];
				}
			}
			send(sock,clients_temp,total*sizeof(struct client),0);
			free(clients_temp);
			pthread_mutex_unlock(&mutex);
		} else if(message.msg_type=='e') {
			pthread_mutex_lock(&mutex);
			i=message.cli.id;
			clients[i].id=-1;
			total--;
			close(sock);
			pthread_mutex_unlock(&mutex);
			pthread_exit(0);
		}
	}
}

int main(int argc, char * argv[]){
	if(argc != 3){
		fprintf(stderr,"%s [port] [sciezka]",argv[0]);
		exit(1);
	}
	unlink(argv[2]);
	sunix=socket(PF_UNIX, SOCK_STREAM, 0);
	sinet=socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_un sunix_addr;
	struct sockaddr_in sinet_addr;
	struct sockaddr_un client_addr_unix;
	struct sockaddr_in client_addr_inet;
	bzero(&sunix_addr,sizeof(sunix_addr));
	bzero(&sinet_addr,sizeof(sinet_addr));
	bzero(&client_addr_unix,sizeof(client_addr_unix));
	bzero(&client_addr_inet,sizeof(client_addr_inet));
	//gniazdo unix
	sunix_addr.sun_family=AF_UNIX;
	strncpy(sunix_addr.sun_path,argv[2],sizeof(sunix_addr.sun_path));
	int size = SUN_LEN(&sunix_addr);
	if(bind(sunix,(struct sockaddr *)&sunix_addr,size) == -1) {
		perror("bind unix error");
		exit(1);
	}
	//gniazdo inet
	sinet_addr.sin_family = AF_INET;
	sinet_addr.sin_port = htons(atoi(argv[1]));
	sinet_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sinet,(struct sockaddr *)&sinet_addr,sizeof(struct sockaddr_in)) == -1) {
		perror("bind inet error");
		exit(1);
	}
	size_unix=sizeof(struct sockaddr_un);
	size_inet=sizeof(struct sockaddr_in);
	if(listen(sunix,10)<0){
		perror("unix listen error");
		exit(1);
	}
	if(listen(sinet,10)<0){
		perror("inet listen error");
		exit(-1);
	}
	int flags = fcntl(sinet, F_GETFL, 0);
	fcntl(sinet, F_SETFL, flags | O_NONBLOCK);
	flags = fcntl(sunix, F_GETFL, 0);
	fcntl(sunix, F_SETFL, flags | O_NONBLOCK);
	int dec=0;
	int i,j;
	int err;
	while(1) {
		int newsock = accept(dec%2==0?sunix:sinet,NULL,NULL);
		if(newsock==-1 && errno!=EWOULDBLOCK){
			perror("accept error");
			exit(1);
		} else if (newsock!=-1){
			struct client cli;
			recv(newsock,&cli,sizeof(struct client),0);
			pthread_mutex_lock(&mutex);
			for(i=0; i<ptr; i++)
				if(clients[i].id==-1)
					break;
			printf("rejestruje klienta %d \n",i);
			clients[i]=cli;
			clients[i].id=num;
			clients[i].socket=newsock;
			send(newsock,&num,sizeof(int),0);
			total++;
			num++;
			if(i==ptr)
				ptr++;
			printf("wysylam liste do klienta %d \n",i);
			send(newsock,&total,sizeof(int),0);
			struct client * clients_temp = malloc(sizeof(struct client)*total);
			int k=0;
			for(j=0; j<ptr; j++){
				if(clients[j].id!=-1){
					clients_temp[k++] = clients[j];
				}
			}
			send(newsock,clients_temp,total*sizeof(struct client),0);
			pthread_mutex_unlock(&mutex);
			pthread_t newthread;
			if((err=pthread_create(&newthread,NULL,sep_thread,&i))<0) {
				fprintf(stderr,"Blad tworzenia nowego watku: %s\n",strerror(err));
				exit(-1);
			}
			if((err=pthread_detach(newthread))<0){
				fprintf(stderr,"Blad odlaczania watku: %s\n",strerror(err));
				exit(-1);
			};
		}
			
		/*
		if(recv(sunix,&message,sizeof(message),MSG_DONTWAIT)!=-1){
			message.cli.addr=(struct sockaddr*)&client_addr_unix;
			analyze(message);
		} else if(errno!=EWOULDBLOCK) {
			perror("unix recv error");
		} else if (recv(sinet,&message,sizeof(message),MSG_DONTWAIT)!=-1){
			message.cli.addr=(struct sockaddr*)&client_addr_inet;
			analyze(message);
		} else if(errno!=EWOULDBLOCK) {
			perror("unix recv error");
		}
		*/
		dec=(dec+1)%2;
	}
	return 0;
}
