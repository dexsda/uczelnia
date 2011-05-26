#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


struct packet {
	int length;
	char *value;
};

char fifo[50];
int fd;
void int_handler(int signo)
{
	close(fd);
	unlink(fifo);
	exit(0);
}


int main(int argc, char *argv[])
{
	if(argc != 2) {
		fprintf(stderr, "%s nazwa_potoku\n", argv[0]);
		exit(-1);
	}
	strcpy(fifo,argv[1]);
	signal(SIGINT, int_handler);
	if(mkfifo(fifo, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP ) == -1) {
		fprintf(stderr, "blad przy tworzeniu kolejki\n");
		exit(-1);
	}
	
	if((fd = open(fifo, O_RDONLY)) == -1) {
		fprintf(stderr, "blad otwarcia kolejki\n");
		exit(-1);
	}
	
	struct packet rec;
	
	for(;;){
		while(!read(fd, &rec.length, sizeof(int)));
		
		rec.value = malloc(rec.length + 1);
		if(!rec.value) {
			fprintf(stderr, "blad pamieci\n");
			exit(-1);
		}
		
		if(read(fd, rec.value, rec.length + 1) == -1)
			exit(-1);

		printf("Odebrano paczke, len: %d%s\n",rec.length, rec.value);
		printf("\nKoniec paczki\n");
		free(rec.value);
	}
	return 0;
}

