#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void print_result(float * vec, int size, int size_sqr){
	int i;
	puts("+++");
	for(i=0;i<size_sqr;i++){
		if(i>0 && i%size==0)
			puts("");
		printf("%8.4f ",vec[i]);
	}
	puts("");
}

int main(int argc, char * argv[])
{
	if(argc!=6) {
		fprintf(stderr,"too few args");
		return -1;
	}
	float boundary=atof(argv[1]);
	int size=atoi(argv[2]);
	int size_sqr=size*size;
	FILE * handle = fopen(argv[3],"r");
	float step=1.0/(float)size;
	float time=atof(argv[4]);
	int tsteps=atoi(argv[5]);
	float timestep=time/(float)tsteps;
	float * x1=calloc(size_sqr,sizeof(float));
	float * x2=calloc(size_sqr,sizeof(float));
	float * temp=NULL;
	float factor=timestep/(step*step);
	int i,j,k;
	float tmp_val;
	for(i=0;i<size_sqr;i++)
		fscanf(handle,"%f",&x1[i]);
	for(k=0;k<tsteps; k++){
		//printf("\nstep %d (time %f)",k,((float)k)*timestep);
		print_result(x1,size,size_sqr);
		for(i=0; i<size; i++){
			for(j=0; j<size; j++){
				tmp_val=0;
				if(i>0) {
					tmp_val+=factor*x1[(i-1)*size+j];
				} else {
					tmp_val+=factor*boundary;
				}
				if(j>0) {
					tmp_val+=factor*x1[i*size+j-1];
				} else {
					tmp_val+=factor*boundary;
				}
				tmp_val+=(1.0-(4.0*factor))*x1[i*size+j];
				if(j<size-1) {
					tmp_val+=factor*x1[i*size+j+1];
				} else {
					tmp_val+=factor*boundary;
				}
				if(i<size-1){
					tmp_val+=factor*x1[(i+1)*size+j];
				} else {
					tmp_val+=factor*boundary;
				}
				x2[i*size+j]=tmp_val;
			}
		}
		temp=x1;
		x1=x2;
		x2=temp;
	}
	//printf("\nstep %d (time %f)",k,((float)k)*timestep);
	print_result(x1,size,size_sqr);
	return 0;
}
