#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

struct packet
{
	int length;
	char *value;
};

int main(int argc, char *argv[])
{
	if(argc != 4) {
		fprintf(stderr, "%s fifo length value\n", argv[0]);
		exit(-1);
	}
	
	char fifo[50];
	strcpy(fifo,argv[1]);
	int length = atoi(argv[2]);
	char character = argv[3][0];
	int fd;
	
	if((fd = open(fifo, O_WRONLY)) == -1) {
		fprintf(stderr, "blad otwarcia potoku\n");
		exit(-1);
	}

	struct packet pack;
	pack.length = length;
	pack.value = malloc(length + 1);
	if(!pack.value)
	{
		fprintf(stderr, "malloc error\n");
		exit(-1);
	}
	pack.value = memset(pack.value, character, length);
	pack.value[length] = '\0';
	
	if(write(fd, &pack.length, sizeof(int)) == -1) {
		fprintf(stderr, "blad zapisu\n");
		exit(-1);
	}
	if(write(fd, pack.value, pack.length + 1) == -1)
	{
		fprintf(stderr, "blad zapisu\n");
		exit(-1);
	}
	
	printf("Wyslano, dlugosc %d, znak %c\n", pack.length, character);
	
	free(pack.value);
	close(fd);
	
	return 0;
}

