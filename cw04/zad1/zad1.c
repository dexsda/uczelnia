#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>

int licznik;
#ifdef CLONE
void * stack;
void * st;
#endif
#ifdef VCLONE
void * stack;
void * st;
#endif

int f(){
	licznik++;
	_exit(0);
}

double do_fork()
{
	double cl1=times(NULL),cl2=0;
#ifdef FORK
	int pid=fork();
#elif VFORK
	int pid=vfork();
#elif CLONE
	int pid=clone(f,stack,CLONE_FS,NULL);
#elif VCLONE
	int pid=clone(f,stack,CLONE_FS|CLONE_SIGHAND|CLONE_VFORK|CLONE_VM,NULL);
#endif
	if(!pid){
		f();
	} else {
		int ext;
#ifdef FORK
		wait(&ext);
#elif VFORK
		wait(&ext);
#else
		waitpid(pid, &ext, __WCLONE);
#endif
		cl2=times(NULL);
	}
	return cl2-cl1;
}

int main(int argc, char * argv[])
{
	struct tms time;
	int i,n=4096;
	double cl1=times(NULL),cl2;
	double c_real=0.0;
	int clk = sysconf(_SC_CLK_TCK);
	if(argc>1)
		n=atoi(argv[1]);
#ifndef FORK
#ifndef VFORK
	stack=malloc(1024);
	st=stack;
	stack+=1024;
#endif
#endif
	for(i=0;i<n; i++)
		c_real+=do_fork();
#ifdef FORK
	printf("fork:\n");
#elif VFORK
	printf("vfork:\n");
#elif CLONE
	printf("clone:\n");
	free(st);
#elif VCLONE
	printf("clone (vfork):\n");
	free(st);
#endif
	cl2=times(&time);
	printf("n              : %d\n",n);
	printf("licznik        : %d\n",licznik);
	printf("real (parent)  : %.2fs\n",(cl2-cl1)/clk);
	printf("real (children): %.2fs\n",c_real/clk);
	printf("user (parent)  : %.2fs\n",(double)time.tms_utime/clk);
	printf("user (children): %.2fs\n",(double)time.tms_cutime/clk);
	printf("sys  (parent)  : %.2fs\n",(double)time.tms_stime/clk);
	printf("sys  (children): %.2fs\n",(double)time.tms_cstime/clk);
	return 0;
}
