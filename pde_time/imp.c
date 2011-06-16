#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float ** init_matrix(float ** mat, int size_sqr){
	int i;
	mat=malloc(sizeof(float *)*size_sqr);
	for(i=0; i<size_sqr;i++)
		mat[i]=calloc(size_sqr,sizeof(float));
	return mat;
}

float ** fill_matrix(float ** mat, int size, int size_sqr,float factor){
	int i;
	/*main diags first*/
	for(i=1;i<size_sqr; i++){
		mat[i][i]=-(4.0*factor+1.0);
		if(i%size!=0)
			mat[i-1][i]=mat[i][i-1]=factor;
	}
	mat[0][0]=-(4.0*factor+1.0);
	/*other diags*/
	for(i=size;i<size_sqr; i++){
		mat[i][i-size]=factor;
		mat[i-size][i]=factor;
	}
	return mat;
}

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

float * gauss_seidel(float ** mat, float * x, float * vec, int size_sqr){
	int i,j,k=0;
	float prev;
	float sum;
	float maxerr=0.01;
	while(maxerr>0.0000001 && k<size_sqr){
		maxerr=0.0;
		for(i=0; i<size_sqr; i++){
			prev=x[i];
			sum=0.0;
			for(j=0;j<i;j++)
				sum+=mat[i][j]*x[j];
			for(j=i+1;j<size_sqr;j++)
				sum+=mat[i][j]*x[j];
			x[i]=(vec[i]-sum)/mat[i][i];
			if(fabs(x[i]-prev)>maxerr)
				maxerr=fabs(x[i]-prev);
		}
		k++;
	}
	return x;
}

float * boundary_cond(float * vec, int size, int size_sqr, float factor, float boundary){
	int i,j;
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			vec[i*size+j]=-vec[i*size+j];
			if(i==0) vec[i*size+j]-=factor*boundary;
			if(j==0) vec[i*size+j]-=factor*boundary;
			if(j==size-1) vec[i*size+j]-=factor*boundary;
			if(i==size-1) vec[i*size+j]-=factor*boundary;
		}
	}
	return vec;
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
	float ** matrix=NULL;
	matrix=init_matrix(matrix,size_sqr);
	float * temp=NULL;
	float factor=timestep/(step*step);
	int i,k;
	matrix=fill_matrix(matrix,size,size_sqr,factor);
	//read file in
	for(i=0;i<size_sqr;i++)
		fscanf(handle,"%f",&x1[i]);
	//do steps
	for(k=0;k<tsteps; k++){
		//printf("\nstep %d (time %f)",k,((float)k)*timestep);
		print_result(x1,size,size_sqr);
		x1=boundary_cond(x1,size,size_sqr,factor,boundary);
		x2=gauss_seidel(matrix,x2,x1,size_sqr);
		temp=x1;
		x1=x2;
		x2=temp;
	}
	//printf("\nstep %d (time %f)",k,((float)k)*timestep);
	print_result(x1,size,size_sqr);
	return 0;
}
