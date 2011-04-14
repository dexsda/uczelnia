#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int licznik;

{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return (double)tv.tv_sec+(double)tv.tv_usec/1000000.0;
}

double do_fork()
{
	double cl1=get_time(),cl2;
	int pid=fork();
	if(!pid){
		licznik++;
		_exit(0);
	} else {
		int ext;
		wait(&ext);
		cl2=get_time();
	}
	return cl2-cl1;
}

int main(int argc, char * argv[])
{
	struct tms time;
	int i,n=4096;
	double cl1=get_time(),cl2;
	double c_real=0.0;
	if(argc>1)
		n=atoi(argv[1]);
	for(i=0;i<n; i++)
		c_real+=do_fork();
	cl2=get_time();
#ifdef FORK
	printf("fork:\n");
#elif VFORK
	printf("vfork:\n");
#elif CLONE
	printf("clone:\n");
#elif VCLONE
	printf("clone (vfork):\n");
#endif
	times(&time);
	printf("real (parent):   %f\n",cl2-cl1);
	printf("real (children): %f\n",c_real);
	return 0;
}
