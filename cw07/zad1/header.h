#include <sys/types.h>
#define NAME 24
#define MAX 100
#define MAXPID 20000

struct msg_client {
	long type;
	int id;
	char name[NAME];
	char msg[MAX];
};

struct msg_server {
	long type;
	ushort msgs;
	ushort cbytes;
	ushort qbytes;
	ushort rpid;
	ushort wpid;
	time_t rtime;
	time_t wtime;
	char name[NAME];
	char msg[MAX];
};

