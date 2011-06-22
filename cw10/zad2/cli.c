#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "header.h"

struct msg message;
struct client cli;
struct sockaddr * server_addr;
int sock;
size_t size;

void getlist(){
	int temp=0;
	if(recv(sock,&temp,sizeof(int),0)==-1) perror("conn error");
	struct client * clients = malloc(sizeof(struct client)*temp);
	if(recv(sock,clients,sizeof(struct client)*temp,0)==-1) perror("conn error");
	int i;
	for(i=0; i<temp; i++)
		printf("id %d, type %d: %s\n",clients[i].id,clients[i].type,clients[i].hostname);
	free(clients);
}

void end(){
	exit(0);
}

int main(int argc, char * argv[]){
	if(argc == 3 && !strcmp(argv[1],"unix")){
		cli.type=UNIX;
		sock=socket(PF_UNIX, SOCK_STREAM, 0);
		struct sockaddr_un server_addr_un;
		bzero(&server_addr_un,sizeof(server_addr_un));
		server_addr_un.sun_family=AF_UNIX;
		strncpy(server_addr_un.sun_path,argv[2],sizeof(server_addr_un.sun_path));
		size = sizeof(server_addr_un);
		server_addr=(struct sockaddr *)&server_addr_un;
	} else if(argc == 4 && !strcmp(argv[1],"inet")){
		cli.type=INET;
		sock=socket(PF_INET, SOCK_STREAM, 0);
		struct sockaddr_in server_addr_in;
		bzero(&server_addr_in,sizeof(server_addr_in));
		server_addr_in.sin_family = AF_INET;
		server_addr_in.sin_port = htons(atoi(argv[3]));
		inet_aton(argv[2],&(server_addr_in.sin_addr));
		size = sizeof(server_addr_in);
		server_addr=(struct sockaddr *)&server_addr_in;
	} else {
		fprintf(stderr,"%s unix path | inet ip port",argv[0]);
		exit(1);
	}
	if(connect(sock,server_addr,size)==-1){
		perror("connect error");
		exit(1);
	}
	struct hostent * host;
	host = gethostent();
	gethostname(cli.hostname,sizeof(cli.hostname));
	strcat(cli.hostname, " ");
	strcat(cli.hostname,host->h_name);
	message.cli = cli;
	message.client_to_stat=0;
	message.msg_type='r';
	if(send(sock,&cli,sizeof(struct client),0)==-1){
		perror("send error");
	}
	int temp=0;
	if(recv(sock,&temp,sizeof(int),0)==-1)
	       perror("conn error");
	cli.id=temp;
	getlist();
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	char buf[3];
	message.cli = cli;
	while(1){
		if(read(0,buf,1)!=-1){
			message.msg_type=buf[0];
			if(buf[0]=='l'){
				if(send(sock,&message,sizeof(struct msg),0)==-1) puts("wtf");
				getlist();
			}
			if(buf[0]=='e'){
				if(send(sock,&message,sizeof(struct msg),0)==-1);
				end();
			}
			if(buf[0]=='i'){
				buf[0]=buf[1]=buf[2]=0;
				read(0,buf,3);
				message.client_to_stat=atoi(buf);
				if(send(sock,&message,sizeof(struct msg),0)==-1);
				buf[0]=buf[1]=buf[2]=0;
				struct info temp_info;
				if(recv(sock,&temp_info,sizeof(struct info),0)==-1) perror("conn error");
				printf("proc: %d, us: %d, load: %f, memt: %d, memf: %d\n",temp_info.proc,temp_info.users,temp_info.load,temp_info.memtot,temp_info.memfree);
			}
		}
		if(recv(sock,&temp,sizeof(int),MSG_DONTWAIT)!=-1){
			struct info stats;
			FILE * p = popen("ps -a | wc -l", "r");
			fscanf(p,"%d",&(stats.proc));
			p = popen("who | awk '{print $1}' | uniq | wc -l", "r");
			fscanf(p,"%d",&(stats.users));
			p = popen("cat /proc/loadavg","r");
			fscanf(p,"%f",&(stats.load));
			p = popen("cat /proc/meminfo","r");
			fscanf(p,"%*s %d",&(stats.memtot));
			fscanf(p,"%*s %*s %d",&(stats.memfree));
			if(send(sock,&stats,sizeof(struct info),0)==-1);
		}
	}
	return 0;
}
