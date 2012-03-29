#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <malloc.h>

/* wykrycie, czy big endian */
#define BIGENDIAN (htons(1)==1)

long long htonll (char * c) {
	long long num;
	int i;
	char *p = (char *)&num;
	for(i=0; i<8; i++)
		p[i] = c[7-i];
	return num;
}

int main(int argc, char * argv[]) {
	if(argc!=2){
		fprintf(stderr,"podaj argument portu");
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in * srv_addr = calloc(1,sizeof(struct sockaddr_in));
	struct sockaddr_in * cli_addr = calloc(1,sizeof(struct sockaddr_in));
	int cli_sock,srv_sock = socket(AF_INET,SOCK_STREAM,0);
	socklen_t len;
	if(srv_sock==-1){
		perror("socket opening error");
		exit(EXIT_FAILURE);
	}
	/* adres */
	srv_addr->sin_family      = AF_INET;
	srv_addr->sin_addr.s_addr = htonl(INADDR_ANY);
	srv_addr->sin_port        = htons(atoi(argv[1]));

	/* komunikacja - poczatek nasluchiwania */
	if(bind(srv_sock,(struct sockaddr*)srv_addr,sizeof(struct sockaddr_in))==-1){
		perror("bind error");
		exit(EXIT_FAILURE);
	}
	if(listen(srv_sock,5)==-1){
		perror("listen error");
		exit(EXIT_FAILURE);
	}

	while(1) {
		len=sizeof(struct sockaddr_in);
		cli_sock = accept(srv_sock,(struct sockaddr*)cli_addr,&len);
		if(cli_sock==-1){
			perror("clisock error");
			exit(EXIT_FAILURE);
		}
		/* bajt - zamiany niepotrzebne */
		char temp_char;
		recv(cli_sock,&temp_char,1,0);
		printf("Received byte: %d\n",temp_char);
		temp_char++;
		send(cli_sock,&temp_char,1,0);
		printf("Sent %d\n",temp_char);
		/* short - zamiana biblioteczna */
		short temp_short=0;
		recv(cli_sock,&temp_short,2,MSG_WAITALL);
		temp_short = ntohs(temp_short);
		printf("Received short: %hd\n",temp_short);
		temp_short++;
		printf("Sent %hd\n",temp_short);
		temp_short = htons(temp_short);
		send(cli_sock,&temp_short,2,0);
		/* int - zamiana biblioteczna */
		int temp_int=0;
		recv(cli_sock,&temp_int,4,MSG_WAITALL);
		temp_int = ntohl(temp_int);
		printf("Received int: %d\n",temp_int);
		temp_int++;
		printf("Sent %d\n",temp_int);
		temp_int = htonl(temp_int);
		send(cli_sock,&temp_int,4,0);
		/* long - zamiana wlasna */
		long long temp_long=0;
		recv(cli_sock,&temp_long,8,MSG_WAITALL);
		if(!BIGENDIAN)
			temp_long=htonll((char *)&temp_long);
		printf("Received dword: %lld\n",temp_long);
		temp_long++;
		printf("Sent dword: %lld\n",temp_long);
		if(!BIGENDIAN)
			temp_long=htonll((char *)&temp_long);
		send(cli_sock,&temp_long,8,0);
	}
	if(close(srv_sock)==-1){
		perror("socket close error");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
