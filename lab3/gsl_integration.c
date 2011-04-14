#include <stdio.h>
#include <math.h>
#include <gsl/gsl_integration.h>
#include <sys/time.h>

double get_time(void)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return (double)tv.tv_sec+(double)tv.tv_usec/1000000.0;
}

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
	double time = get_time();
	printf("Wlasna metoda prostokatow (blad %g): \n",abserror);
	result = square_integrate(f,0,2,expected,abserror,&intervals);
	printf ("result          = % .18f\n", result);
	printf ("exact result    = % .18f\n", expected);
	printf ("actual error    = % .18f\n", result - expected);
	printf ("intervals       =  %d\n", intervals);
	printf ("time            =  %f\n", get_time() - time);
}

void integrate_gsl_qags(double expected, double abserror)
{
	gsl_integration_workspace * w = gsl_integration_workspace_alloc (1000);
	double result, error;
	double alpha = 1.0;
	gsl_function F;
	double time = get_time();
	printf("QAGS GSL (blad %g): \n",abserror);
	F.function = &f;
	F.params = &alpha;
	gsl_integration_qags (&F, 0, 2, abserror, abserror, 1000, w, &result, &error); 
	printf ("result          = % .18f\n", result);
	printf ("exact result    = % .18f\n", expected);
	printf ("estimated error = % .18f\n", error);
	printf ("actual error    = % .18f\n", result - expected);
	printf ("intervals       =  %d\n", w->size);
	printf ("time            =  %f\n", get_time() - time);
	gsl_integration_workspace_free (w);
}

void integrate_gsl_qag(double expected, double abserror,int key)
{
	gsl_integration_workspace * w = gsl_integration_workspace_alloc (1000);
	double result, error;
	double alpha = 1.0;
	gsl_function F;
	double time = get_time();
	printf("QAG GSL (blad %g, klucz %d): \n",abserror,key);
	F.function = &f;
	F.params = &alpha;
	gsl_integration_qag (&F, 0, 2, abserror, abserror, 1000, key, w, &result, &error); 
	printf ("result          = % .18f\n", result);
	printf ("exact result    = % .18f\n", expected);
	printf ("estimated error = % .18f\n", error);
	printf ("actual error    = % .18f\n", result - expected);
	printf ("intervals       =  %d\n", w->size);
	printf ("time            =  %f\n", get_time() - time);
	gsl_integration_workspace_free (w);
}

int main (int argc, char * argv[])
{
	double expected = -1.841116916640328;
	double abserror;
	int i;
	if(argc<2)
		abserror = 1e-2;
	else
		abserror = atof(argv[1]);
	integrate_mine(expected, abserror);
	integrate_gsl_qags(expected, abserror);
	for(i=1; i<=6; i++)
		integrate_gsl_qag(expected, abserror, i);
	return 0;
}
