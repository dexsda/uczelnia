#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>

#ifdef KWADRATOWE
#include "kwadratowe.c"
#elif TRYGONOMETRYCZNE
#include "trygonometryczne.c"
#elif CZWARTY_STOPIEN
#include "czwarty_stopien.c"
#endif

int main() {
	int status;
	int iter = 0, max_iter = 100;
	const double dokladnosc = 1.0e-3;
#ifdef BRACKETING
	const gsl_root_fsolver_type *typ;
	gsl_root_fsolver *solver;
	gsl_function funkcja;
	
	funkcja.function = &f;
	funkcja.params = &wsp;
#ifdef BISECTION
	typ = gsl_root_fsolver_bisection;
#elif FALSEPOS
	typ = gsl_root_fsolver_falsepos;
#elif BRENT
	typ = gsl_root_fsolver_bisection;
#endif
	solver = gsl_root_fsolver_alloc(typ);
	gsl_root_fsolver_set(solver, &funkcja, x_min, x_max);
	printf("%10s [%11s, %11s] %11s %11s\n", "iteracja", "x_min", "x_max", "pierwiastek", "blad_wzgl");
	do {
		++iter;
		status = gsl_root_fsolver_iterate(solver);
		root0 = root;
		root = gsl_root_fsolver_root(solver);
		x_min = gsl_root_fsolver_x_lower(solver);
		x_max = gsl_root_fsolver_x_upper(solver);
		//status = gsl_root_test_interval(x_min, x_max, 0, 0.001);
		status = gsl_root_test_delta(root, root0, 0, dokladnosc);
		if (status == GSL_SUCCESS)
			printf("Osiagnieto podana zbieznosc:\n");
		printf("%10d [%11.7f, %11.7f] %11.7f %11.7f\n", iter, x_min, x_max, root, (root - root0)/root);
	} while (status == GSL_CONTINUE && iter < max_iter);
	gsl_root_fsolver_free(solver);
	return status;
#endif 
#ifdef POLISHING
	const gsl_root_fdfsolver_type *typ;
	gsl_root_fdfsolver *solver;
	gsl_function_fdf funkcja;

	funkcja.f = &f;
	funkcja.df = &df;
	funkcja.fdf = &fdf;
	funkcja.params = &wsp;
#ifdef NEWTON
	typ = gsl_root_fdfsolver_newton;
#elif SECANT
	typ = gsl_root_fdfsolver_secant;
#elif STEFFENSON
	typ = gsl_root_fdfsolver_steffenson;
#endif
	solver = gsl_root_fdfsolver_alloc(typ);
	gsl_root_fdfsolver_set(solver, &funkcja, root);
	printf("%10s %11s %11s\n", "iteracja", "pierwiastek", "blad_wzgl");
	do {
		++iter;
		status = gsl_root_fdfsolver_iterate(solver);
		root0 = root;
		root = gsl_root_fdfsolver_root(solver);
		status = gsl_root_test_delta(root, root0, 0, dokladnosc);
		if (status == GSL_SUCCESS)
			printf("Osiagnieto podana zbieznosc:\n");
		printf("%10d %11.7f %11.7f\n", iter, root, (root - root0)/root);
	} while (status == GSL_CONTINUE && iter < max_iter);
	gsl_root_fdfsolver_free(solver);
	return status;
#endif
}

