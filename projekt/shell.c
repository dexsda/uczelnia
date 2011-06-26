#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include "packets.h"

struct daemon ds[50];
int last;
int slp;
int params[50];

void * conn_thread(void * data){
	int index = *((int*)data);
	while(1){
		struct packet pckt;
		strcpy(pckt.message,"info");
		if(send(ds[index].sock,&pckt,sizeof(struct packet),0)==-1){
			perror("send problem");
		}
		pckt.message[0]='\0';
		recv(ds[index].sock,&pckt,sizeof(struct packet),0);
		printf("\ndaemon %d status: %s",index,pckt.message);
		pckt.message[0]='\0';
		sleep(slp);
	}
}

int main(int argc, char * argv[]){
	char string[1024];
	struct packet pckt;
	int i;
	while(1){
		printf(">> ");
		fgets(string,1024,stdin);
		if(!strncmp(string,"connect",7)){
			printf("Connecting...\n");
			char * pch;
			i=0;
			while(i<last && ds[i].sock!=-1)
				i++;
			int sock=socket(PF_INET, SOCK_STREAM, 0);
			struct sockaddr_in * addr=calloc(1,sizeof(struct sockaddr_in));
			addr->sin_family=AF_INET;
			pch = strtok (string," :");
			pch = strtok (NULL, " :");
			inet_aton(pch,&(addr->sin_addr));
			strcpy(ds[i].ip,pch);
			pch = strtok (NULL, " :");
			addr->sin_port=htons(atoi(pch));
			ds[i].port=atoi(pch);
			if(connect(sock,(struct sockaddr *)addr,sizeof(struct sockaddr_in))==-1){
				printf("Connection error!");
				close(sock);
			} else {
				ds[i].sock=sock;
				if(i==last)
					last++;
			}
		} else if(!strncmp(string,"disconnect",10)){
			printf("Disconnecting...\n");
			char * pch;
			pch = strtok (string," ");
			pch = strtok (NULL, " ");
			if(!strncmp(pch,"*",1)){
				for(i=0; i<last; i++){
					close(ds[i].sock);
					ds[i].sock=-1;
				}
				printf("Disconnected all!\n");
			} else {
				i=atoi(pch);
				if(i<last){
					close(ds[i].sock);
					ds[i].sock=-1;
				}
				printf("Disconnected!\n");
			}
		} else if(!strncmp(string,"list",4)){
			for(i=0; i<last; i++)
				if(ds[i].sock!=-1)
					printf("daemon %d = %s:%d\n",i,ds[i].ip,ds[i].port);
		} else if(!strncmp(string,"configure",9)){
			char * pch;
			pch = strtok (string," ");
			pch = strtok (NULL, " ");
			i=atoi(pch);
			strcpy(pckt.message,"conf");
			pch = strtok (NULL, " ");
			while(pch!=NULL){
				strcat(pckt.message," ");
				strcat(pckt.message,pch);
				pch = strtok (NULL, " ,.-");
			}
			if(i<last && ds[i].sock!=-1){
				printf("Sending configure message...\n");
				send(ds[i].sock,&pckt,sizeof(struct packet),0);
			} else {
				printf("No such daemon!\n");
			}
		} else if(!strncmp(string,"start",5)){
			char * pch;
			pch = strtok (string," ");
			pch = strtok (NULL, " ");
			if(pch!=NULL)
				slp=atoi(pch);
			if(slp==0)
				slp=10;
			for(i=0; i<last; i++){
				if(ds[i].sock!=-1){
					params[i]=i;
					pthread_t newthread;
					if(pthread_create(&newthread,NULL,conn_thread,&(params[i]))<0) {
						fprintf(stderr,"pthread_create error\n");
						exit(EXIT_FAILURE);
					}
					if(pthread_detach(newthread)<0){
						fprintf(stderr,"pthread_detach error\n");
						exit(EXIT_FAILURE);
					}
				}
			}
		} else {
			printf("incorrect command!");
		}
	}
	exit(EXIT_SUCCESS);
}

