#include <stdio.h>
#include <math.h>
#include <gsl/gsl_integration.h>

double f (double x, void * params) {
	double f = log(x*x)+log(x);
	return f;
}

double square_integrate(double func(double, void *), double a, double b, double expected, double abserror, int * intervals)
{
	double h=(b-a);
	double i=a;
	double result=0.0;
	*intervals=1;
	while(fabs(result-expected)>fabs(abserror)){
		result=0.0;
		i=a;
		while(i<b){
			result+=h*f(i+(h/2.0),NULL);
			i+=h;
		}
		h/=2;
		(*intervals)=(*intervals)*2;
	}
	return result;
}

void integrate_mine(double expected, double abserror)
{
	double result;
	int intervals;
	result = square_integrate(f,0,2,expected,abserror,&intervals);
	printf ("result          = % .18f\n", result);
	printf ("exact result    = % .18f\n", expected);
	printf ("actual error    = % .18f\n", result - expected);
	printf ("intervals       =  %d\n", intervals);
}

int main (int argc, char * argv[])
{
	double expected = -1.841116916640328;
	double abserror;
	if(argc<2)
		abserror = 1e-2;
	else
		abserror = atof(argv[1]);
	integrate_mine(expected, abserror);
	return 0;
}
