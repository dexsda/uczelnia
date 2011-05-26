#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

#define READ  0
#define WRITE 1
#define BUFSIZE 256 //rozmiar bufora na przesylany komunikat


int main(int argc, char *argv[])
{
	if(argc > 2 ) {
		fprintf(stderr, "Poprawne wywolanie: zad1 {0-3}, 0 - bez flagi, 1 - flaga na czytelniku, 2 - flaga na pisarzu, 3 - czytelnik & pisarz\n");
		exit(-1);
	}
	char *message = "Wiadomosc";
	int mode=0;
	if(argc==2)
		mode = atoi(argv[1]);

	if(mode < 0 || mode > 3) {
		fprintf(stderr, "Bledny argument\n");
		exit(-1);
	}
	int fd[2];
	char buffer[BUFSIZE];
	if(pipe(fd) == -1) {
		fprintf(stderr, "blad pipe\n");
		exit(-1);
	}
	if(fork() == 0) {
		close(fd[WRITE]);
		int bytes_read;
		
		if(mode==1 || mode==3)
			fcntl(fd[READ], F_SETFL, O_NONBLOCK);
		
		for(;;) {
			if((bytes_read = read(fd[READ], buffer, BUFSIZE)) == -1 && errno == EAGAIN) {
				printf("Potok pusty\n");
				sleep(1);
			} else if(bytes_read == 0) {
				printf("Proces pisarza zakonczyl dzialanie. Koniec procesu czytelnika.\n");
				close(fd[READ]);
				exit(0);
			} else {
				printf("Odczytano:\t%s\n", buffer);
#ifdef SLEEP
				sleep(1);
#endif
			}
		}
	} else {
		close(fd[READ]);
		int bytes_written;
		
		if(mode==2 || mode==3)
			fcntl(fd[WRITE], F_SETFL, O_NONBLOCK);
		
		for(;;) {
			if((bytes_written = write(fd[WRITE], message, strlen(message))) == -1)
			{
				if(errno == EPIPE) {
					printf("Proces czytelnika zakonczyl dzialanie. Koniec procesu pisarza\n");
					close(fd[WRITE]);
					exit(0);
				} else if(errno == EAGAIN) {
					printf("Potok zapelniony\n");
					sleep(1);
				} else {
					fprintf(stderr, "blad zapisu\n");
					close(fd[WRITE]);
					exit(-1);
				}
			} else {
				printf("Zapisano %d bajtow:\t%s\n", bytes_written, message);
#ifdef SLEEP
				sleep(1);
#endif
			}
		}
	}
	return 0;
}

