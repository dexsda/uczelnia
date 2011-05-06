struct wsp_czwar {
	double a, b, c, d, e;
};

struct wsp_czwar wsp = {1.0, -9.0, -43.0, 393.0, -630.0}; // W(x) = (x - 3)^2 * (x + 7) * (x - 10)
double x_min = -2.0, x_max = 7.0;
double root0, root = 9.0;

double f(double x, void *wspolczynniki) {
	struct wsp_czwar *wsp = (struct wsp_czwar*) wspolczynniki;
	double a = wsp->a;
	double b = wsp->b;
	double c = wsp->c;
	double d = wsp->d;
	double e = wsp->e;
	return (((a * x + b) * x + c) * x + d) * x + e;
}

double df(double x, void *wspolczynniki) {
	struct wsp_czwar *wsp = (struct wsp_czwar*) wspolczynniki;
	double a = wsp->a;
	double b = wsp->b;
	double c = wsp->c;
	double d = wsp->d;
	return ((4.0 * a * x + 3.0 * b * x) * x + 2.0 * c) * x + d;
}

void fdf(double x, void *wspolczynniki, double *y, double *dy) {
	struct wsp_czwar *wsp = (struct wsp_czwar*) wspolczynniki;
	double a = wsp->a;
	double b = wsp->b;
	double c = wsp->c;
	double d = wsp->d;
	double e = wsp->e;
	*y = (((a * x + b) * x + c) * x + d) * x + e;
	*dy = ((4.0 * a * x + 3.0 * b * x) * x + 2.0 * c) * x + d;
}
