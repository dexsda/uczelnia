#include<cmath>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<iostream>
#include<gsl/gsl_errno.h>
#include<gsl/gsl_matrix.h>
#include<gsl/gsl_odeiv.h>
#include<gsl/gsl_const_mksa.h>

using namespace std;

#define G GSL_CONST_MKSA_GRAVITATIONAL_CONSTANT

struct wsp3D
{
	double x;
	double y;
	double z;
	wsp3D(){}
	wsp3D(double x1,double y1,double z1) : x(x1),y(y1),z(z1){}
};

class GrawitonyAbstrakcyjne
{
	public:
		GrawitonyAbstrakcyjne(){}
		~GrawitonyAbstrakcyjne(){}
		wsp3D punkt;
		wsp3D predkosc;
		double M;
		double krok;
		virtual void oblicz(){};
		void inject(wsp3D pun, wsp3D pred,double M1,double krok1)
		{
			punkt.x=pun.x;
			punkt.y=pun.y;
			punkt.z=pun.z;
			predkosc.x=pred.x;
			predkosc.y=pred.y;
			predkosc.z=pred.z;
			M=M1;
			krok=krok1;
		}
};
int GrawitonWrapper(double t, const double x[], double f[], void *params);
class GrawitonyGSL : public GrawitonyAbstrakcyjne
{	public:	
	const gsl_odeiv_step_type * typ;
	gsl_odeiv_step * step;
	double czas;
	double wej[6];
	double wyj[6];
	double y[6];
	double yerror[6];

	GrawitonyGSL()
	{	
		czas=0.0;
		typ=gsl_odeiv_step_rk4;
		step=gsl_odeiv_step_alloc(typ,6);


	}
	~GrawitonyGSL()
	{
		gsl_odeiv_step_free(step);
	}

	void oblicz() 
	{	
		gsl_odeiv_system sys;
		if(czas==0)
		{
			y[0]=punkt.x;
			y[1]=predkosc.x;
			y[2]=punkt.y;
			y[3]=predkosc.y;
			y[4]=punkt.z;
			y[5]=predkosc.z;
		}
		sys.function=GrawitonWrapper;
		sys.jacobian=0;
		sys.dimension=6;
		sys.params=&M;
		GSL_ODEIV_FN_EVAL(&sys, czas, y, wej);
		int status;
		if((status = gsl_odeiv_step_apply(step,czas,krok,y,yerror,wej,wyj,&sys))!=GSL_SUCCESS)
		{
			perror("Blad w apply!");
			exit(-1);
		}
		wej[0] = wyj[0];
		wej[1] = wyj[1];
		wej[2] = wyj[2];
		wej[3] = wyj[3];
		wej[4] = wyj[4];
		wej[5] = wyj[5];
		czas+=krok;
		cout<<y[0]<<" "<<y[2]<<" "<<y[4]<<endl;

	}
	int f(double t, const double y[], double dydt[], void *params) 
	{
		double tmpM=*(double *) params;
		double r=sqrt(y[0]*y[0] + y[2]*y[2] + y[4]*y[4]);
		dydt[0] = y[1];
		dydt[1] = -y[0]*G*tmpM/(r*r*r);
		dydt[2] = y[3];
		dydt[3] = -y[2]*G*tmpM/(r*r*r);
		dydt[4] = y[5];
		dydt[5] = -y[4]*G*tmpM/(r*r*r);
		return GSL_SUCCESS;
	}
};

int GrawitonWrapper(double t, const double x[], double f[], void *params)
{

	GrawitonyGSL * graw;
	return graw->f(t, x, f, params);
}

int main(int argc, char** argv) 
{
	if(argc<9)
	{
		printf("Usage: ./grav [pos vect] [sp vect] [step] [mass] \n");
		return -1;
	}
	GrawitonyAbstrakcyjne * grawitacja;
	grawitacja=new GrawitonyGSL();
	wsp3D p,v;
	double k, M;
	p.x=atof(argv[1]);
	p.y=atof(argv[2]);
	p.z=atof(argv[3]);
	v.x=atof(argv[4]);
	v.y=atof(argv[5]);
	v.z=atof(argv[6]);
	k=atof(argv[7]);
	M=atof(argv[8]);
	cout<<M<<endl;
	grawitacja->inject(p, v, M, k);
	while(1) {
		grawitacja->oblicz();
	}

	return 0;
}

