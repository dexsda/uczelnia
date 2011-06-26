struct packet {
	char message[1024];
};

struct daemon {
	int sock;
	char ip[15];
	int port;
};
