#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "packets.h"

int sock;
FILE * logfile;
int ps,users,mem,loadavg,threads;

void sigint_handler(int signo){
	close(sock);
	exit(EXIT_SUCCESS);
}

int conf(char * buf, char * str){
	if(strstr(buf,str)){
		return 1;
	} else {
		return 0;
	}
}

void * conn_thread(void * data){
	int sock = *((int*)data);
	while(1){
		struct packet pckt;
		recv(sock,&pckt,sizeof(struct packet),0);
		if (!strncmp(pckt.message,"conf",4)){
			ps=conf(pckt.message,"ps");
			users=conf(pckt.message,"users");
			mem=conf(pckt.message,"mem");
			loadavg=conf(pckt.message,"load");
			threads=conf(pckt.message,"threads");
			fprintf(logfile,"%s\n",pckt.message);
		} else if(!strncmp(pckt.message,"info",4)) {
			pckt.message[0]='\0';
			if(ps){
				int temp;
				FILE * p = popen("ps -a | wc -l", "r");
				fscanf(p,"%d",&(temp));
				pclose(p);
				sprintf(pckt.message,"%s | processes: %d",pckt.message,temp);
			}
			if(users){
				int temp;
				FILE * p = popen("who | awk '{print $1}' | uniq | wc -l", "r");
				fscanf(p,"%d",&(temp));
				pclose(p);
				sprintf(pckt.message,"%s | users: %d",pckt.message,temp);
			}
			if(loadavg){
				float temp;
				FILE * p = popen("cat /proc/loadavg", "r");
				fscanf(p,"%f",&(temp));
				pclose(p);
				sprintf(pckt.message,"%s | loadavg: %f",pckt.message,temp);
			}
			if(mem){
				int temp1,temp2;
				FILE * p = popen("cat /proc/meminfo", "r");
				fscanf(p,"%*s %d %*s %*s %d",&(temp1),&(temp2));
				pclose(p);
				sprintf(pckt.message,"%s | mem: total[%dB]/free[%dB]",pckt.message,temp1,temp2);
			}
			if(threads){
				int temp;
				FILE * p = popen("ps -eLf | wc -l", "r");
				fscanf(p,"%d",&(temp));
				pclose(p);
				sprintf(pckt.message,"%s | threads: %d",pckt.message,temp);
			}
			fprintf(logfile,"%s\n",pckt.message);
			send(sock,&pckt,sizeof(struct packet),0);
		}
		pckt.message[0]='\0';
		sleep(1);
	}
}

int main(int argc, char * argv[]) {
	/* przyzywamy demona */
	int port=2048;
	if(argc==2)
		port=atoi(argv[1]);
	if(port<=2000){
		fprintf(stderr,"port too low");
		exit(EXIT_FAILURE);
	}
	pid_t pid, sid;
	pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}
	umask(0);
	sid = setsid();
	if (sid < 0) {
		exit(EXIT_FAILURE);
	}
	if ((chdir("/")) < 0) {
		exit(EXIT_FAILURE);
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	/* inicjalizacja */
	signal(SIGTERM,sigint_handler);
	logfile=fopen("/home/dex/development/uczelnia/projekt/log.txt","w");
	sock=socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1){
		fprintf(logfile,"socket creation error\n");
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in * addr=calloc(1,sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sock,(struct sockaddr *)addr,sizeof(struct sockaddr_in))==-1){
		fprintf(logfile,"socket bind error\n");
		exit(EXIT_FAILURE);
	}
	if(listen(sock,10)<0){
		fprintf(logfile,"socket listen error\n");
		exit(EXIT_FAILURE);
	}
	while (1) {
		fprintf(logfile,"waiting\n");
		int newsock=accept(sock,NULL,NULL);
		if(newsock==-1){
			fprintf(logfile,"socket error\n");
			exit(EXIT_FAILURE);
		} else {
			fprintf(logfile,"connection accepted\n");
			pthread_t newthread;
			if(pthread_create(&newthread,NULL,conn_thread,&newsock)<0) {
				fprintf(logfile,"pthread_create error\n");
				exit(EXIT_FAILURE);
			}
			if(pthread_detach(newthread)<0){
				fprintf(logfile,"pthread_detach error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	exit(EXIT_SUCCESS);
}
