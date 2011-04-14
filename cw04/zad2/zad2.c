#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void backup(char * filename, char * backup_filename)
{
	int ex;
	puts("making a backup file...");
	if(!fork())
		execlp("cp","cp","-pf",filename,backup_filename,NULL);
	wait(&ex);
}

void revert(char * filename, char * backup_filename)
{
	int ex;
	puts("reverting changes...");
	if(!fork())
		execlp("cp","cp","-pf",backup_filename,filename,NULL);
	wait(&ex);
}

int main(int argc, char * argv[])
{
	if(argc!=3){
		fprintf(stderr,"Zla ilosc argumentow! Poprawnie: monfile <plik> <czas>");
		return 1;
	}
	struct stat * stats;
	stats=malloc(sizeof(struct stat));
	int sec=atoi(argv[2]);
	backup(argv[1],"___backup");
	lstat("___backup",stats);
	int mod_date=stats->st_mtime;
	while(1){
		lstat(argv[1],stats);
		if(stats->st_mtime!=mod_date)
			revert(argv[1],"___backup");
		sleep(sec);
	}
	free(stats);
	return 0;
}
