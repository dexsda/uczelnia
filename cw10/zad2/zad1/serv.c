#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <unistd.h>
#include "header.h"

struct msg message;

struct client clients[MAX_CLIENT];
size_t size_unix;
size_t size_inet;
int total,ptr;
int num;
int sunix,sinet;

void analyze(struct msg message){
	int i,j;
	if(message.msg_type=='r'){
		for(i=0; i<ptr; i++)
			if(clients[i].id==-1)
				break;
		printf("rejestruje klienta %d \n",i);
		clients[i]=message.cli;
		clients[i].id=num;
		if(clients[i].type==UNIX)
			sendto(sunix,&num,sizeof(int),0,clients[i].addr,size_unix);
		else
			sendto(sinet,&num,sizeof(int),0,clients[i].addr,size_inet);
		total++;
		num++;
		if(i==ptr)
			ptr++;
		printf("wysylam liste do klienta %d \n",i);
		if(clients[i].type==UNIX)
			sendto(sunix,&total,sizeof(int),0,clients[i].addr,size_unix);
		else
			sendto(sinet,&total,sizeof(int),0,clients[i].addr,size_inet);
		struct client * clients_temp = malloc(sizeof(struct client)*total);
		int k=0;
		for(j=0; j<ptr; j++){
			if(clients[j].id!=-1){
				clients_temp[k++] = clients[j];
			}
		}
		if(clients[i].type==UNIX)
			sendto(sunix,clients_temp,total*sizeof(struct client),0,clients[i].addr,size_unix);
		else
			sendto(sinet,clients_temp,total*sizeof(struct client),0,clients[i].addr,size_inet);
		free(clients_temp);
	} else if(message.msg_type=='i'){
		i=message.cli.id;
		j=message.client_to_stat;
		if(j<ptr && clients[j].id!=-1){
			printf("odpytuje klienta %d dla klienta %d\n",j,i);
			int temp='z';
			struct info stats_temp;
			if(clients[j].type==UNIX){
				sendto(sunix,&temp,sizeof(int),0,clients[j].addr,size_unix);
				recvfrom(sunix,&stats_temp,sizeof(struct info),0,NULL,NULL);
			} else {
				sendto(sinet,&temp,sizeof(int),0,clients[j].addr,size_inet);
				recvfrom(sinet,&stats_temp,sizeof(struct info),0,NULL,NULL);
			}
			if(clients[i].type==UNIX)
				sendto(sunix,&stats_temp,sizeof(struct info),0,clients[i].addr,size_unix);
			else
				sendto(sinet,&stats_temp,sizeof(struct info),0,clients[i].addr,size_inet);
		} else {
			puts("nie ma klienta");
		}
	} else if(message.msg_type=='l'){
		i=message.cli.id;
		printf("wysylam liste do klienta %d \n",i);
		if(clients[i].type==UNIX)
			sendto(sunix,&total,sizeof(int),0,clients[i].addr,size_unix);
		else
			sendto(sinet,&total,sizeof(int),0,clients[i].addr,size_inet);
		struct client * clients_temp = malloc(sizeof(struct client)*total);
		int k=0;
		for(j=0; j<ptr; j++){
			if(clients[j].id!=-1){
				clients_temp[k++] = clients[j];
			}
		}
		if(clients[i].type==UNIX)
			sendto(sunix,clients_temp,total*sizeof(struct client),0,clients[i].addr,size_unix);
		else
			sendto(sinet,clients_temp,total*sizeof(struct client),0,clients[i].addr,size_inet);
		free(clients_temp);
	} else if(message.msg_type=='e') {
		i=message.cli.id;
		clients[i].id=-1;
		total--;
	}
}

int main(int argc, char * argv[]){
	if(argc != 3){
		fprintf(stderr,"%s [port] [sciezka]",argv[0]);
		exit(1);
	}
	unlink(argv[2]);
	sunix=socket(PF_UNIX, SOCK_DGRAM, 0);
	sinet=socket(PF_INET, SOCK_DGRAM, 0);
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
		exit(-1);
	}
	//gniazdo inet
	sinet_addr.sin_family = AF_INET;
	sinet_addr.sin_port = htons(atoi(argv[1]));
	sinet_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sinet,(struct sockaddr *)&sinet_addr,sizeof(struct sockaddr_in)) == -1) {
		perror("bind inet error");
		exit(-1);
	}
	size_unix=sizeof(struct sockaddr_un);
	size_inet=sizeof(struct sockaddr_in);
	while(1) {
		if(recvfrom(sunix,&message,sizeof(message),MSG_DONTWAIT,(struct sockaddr *)&client_addr_unix,&size_unix)!=-1){
			message.cli.addr=(struct sockaddr*)&client_addr_unix;
			analyze(message);
		} else if(errno!=EWOULDBLOCK) {
			perror("unix recv error");
		} else if (recvfrom(sinet,&message,sizeof(message),MSG_DONTWAIT,(struct sockaddr*)&client_addr_inet,&size_inet)!=-1){
			message.cli.addr=(struct sockaddr*)&client_addr_inet;
			analyze(message);
		} else if(errno!=EWOULDBLOCK) {
			perror("unix recv error");
		}
		sleep(1);
	}
	return 0;
}
