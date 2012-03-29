#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <malloc.h>

#define SIZE 1024

int main(int argc, char * argv[]) {
	if(argc!=3){
		printf("wrong number of arguments");
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

	//while(1) {
		len=sizeof(struct sockaddr_in);
		cli_sock = accept(srv_sock,(struct sockaddr*)cli_addr,&len);
		if(cli_sock==-1){
			perror("clisock error");
			exit(EXIT_FAILURE);
		}
		char * buffer;
		FILE * input_file = fopen(argv[2],"rb");
		if (input_file==NULL) {
			perror("fopen error");
			exit (EXIT_FAILURE);
		}
		fseek(input_file,0,SEEK_END); //eof
		int size = ftell(input_file);
		size=htonl(size);
		send(cli_sock,&size,4,0); //wysylamy rozmiar, zeby druga strona wiedziala, jak wielki bufor jest potrzebny
		size=ntohl(size);
		rewind (input_file);
		buffer = malloc (sizeof(char)*size);
		if(buffer==NULL){
			perror("malloc error");
			exit(EXIT_FAILURE);
		}
		size_t bytes_read = fread(buffer,1,size,input_file);
		if(bytes_read!=size){
			perror("fread error");
			exit(EXIT_FAILURE);
		}
		char * ptr = buffer;
		while(bytes_read>0){ //wysylamy pakiety jak leci
			int bytes = send(cli_sock,ptr,bytes_read,0);
			if(bytes<=0){
				perror("send error");
				exit(EXIT_FAILURE);
			}
			bytes_read-=bytes;
			ptr+=bytes;
		}
		fclose(input_file);
		free(buffer);
		close(cli_sock);
	//}

	if(close(srv_sock)==-1){
		perror("socket close error");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
