#define MAX 100

struct msg_serv{
	char name[12];
	char message[MAX];
	int ls;
	int number;
};

struct msg_cli{
	int pid;
	char name[12];
	char message[MAX];
};

