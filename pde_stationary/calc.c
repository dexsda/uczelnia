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

float ** fill_matrix(float ** mat, int size, int size_sqr, float step,int x1, int y1, int x2, int y2){
	float diags=1.0/(step*step);
	float diag_main=-diags*4;
	int i;
	/*main diags first*/
	for(i=1;i<size_sqr; i++){
		mat[i][i]=diag_main;
		if(i%size!=0)
			mat[i-1][i]=mat[i][i-1]=diags;
	}
	mat[0][0]=diag_main;
	/*other diags*/
	for(i=size;i<size_sqr; i++){
		mat[i][i-size]=diags;
		mat[i-size][i]=diags;
	}
	//make center stationary
	int temp,j,k;
	for(i=y1;i<=y2;i++){
		for(j=x1;j<=x2;j++){
			temp=i*size+j;
			for(k=0; k<temp; k++) 
				mat[temp][k]=0;
			mat[temp][temp]=diags;
			for(k=temp+1; k<size_sqr; k++) 
				mat[temp][k]=0;
		}
	}
	return mat;
}

float * fill_free_vector(float * vec, int size, int size_sqr, float init, float step, int x1, int y1, int x2, int y2){
	int i,j;
	for(i=y1;i<=y2;i++)
		for(j=x1;j<=x2;j++)
			vec[i*size+j]=init/(step*step);
	return vec;
}

void print_matrix(float ** mat, int size_sqr){
	int i,j;
	for(i=0;i<size_sqr;i++){
		for(j=0;j<size_sqr;j++)
			printf("%8.4f ",mat[i][j]);
		puts("");
	}
}

void print_vector(float * vec, int size_sqr){
	int i;
	for(i=0; i<size_sqr; i++)
		printf("%8.4f ",vec[i]);
	puts("");
}

void print_result(float * vec, int size, int size_sqr){
	int i;
	for(i=0;i<size_sqr;i++){
		if(i>0 && i%size==0)
			puts("");
		printf("%8.4f ",vec[i]);
	}
	puts("\n");
}

void print_all(float * x, float * vec, float ** mat, int size_sqr){
	int i,j;
	puts("+++");
	for(i=0; i<size_sqr; i++){
		for(j=0; j<size_sqr; j++)
			printf("%8.4f ",mat[i][j]);
		printf(i==size_sqr/2 ? " * " : "   ");
		printf("%8.4f ",x[i]);
		printf(i==size_sqr/2 ? " = " : "   ");
		printf("%8.4f\n",vec[i]);
	}
}

void gauss_seidel(float ** mat, float * x, float * vec, int size_sqr){
	int i,j,k=0;
	float prev;
	float sum;
	float maxerr=0.01;
	while(maxerr>0.0001){
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
		/*print_all(x,vec,mat,size_sqr);*/
		/*printf("error: %f",maxerr);*/
		k++;
	}
//	return x;
}

int main(int argc, char * argv[])
{
	if(argc!=7) {
		fprintf(stderr,"too few args");
		return -1;
	}
	float boundary=atof(argv[1]);
	int size=atoi(argv[2]);
	int size_sqr=size*size;
	int x1=atoi(argv[3]);
	int y1=atoi(argv[4]);
	int x2=atoi(argv[5]);
	int y2=atoi(argv[6]);
	float len = size;//atof(argv[3]);
	float step=len/(float)size;
	float ** mat=NULL;
	mat = init_matrix(mat,size_sqr);
	float * vec=calloc(size_sqr,sizeof(float));
	float * x=calloc(size_sqr,sizeof(float));
	vec = fill_free_vector(vec,size,size_sqr,boundary,step,x1,y1,x2,y2);
	//print_result(vec,size,size_sqr);
	mat = fill_matrix(mat,size,size_sqr,step,x1,y1,x2,y2);
	//print_matrix(mat,size_sqr);
	//print_all(x,vec,mat,size_sqr);
	gauss_seidel(mat,x,vec,size_sqr);
	print_result(x,size,size_sqr);
	return 0;
}
