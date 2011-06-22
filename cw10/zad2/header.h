#define MAX_CLIENT 50
#define UNIX 0
#define INET 1

struct client {
	int type;
	int id;
	int socket;
	char hostname[256];
};

struct msg {
	struct client cli;
	int msg_type;
	int client_to_stat;
};

struct info {
	int proc;
	int users;
	float load;
	int memtot;
	int memfree;
};
