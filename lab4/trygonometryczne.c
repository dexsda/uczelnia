struct wsp_tryg {
	double a;
};

struct wsp_tryg wsp = {1.0};
double x_min = 1.0, x_max = 2.0;
double root0, root = 2.0;

double f(double x, void *wspolczynniki) {
	struct wsp_tryg *wsp = (struct wsp_tryg*) wspolczynniki;
	double a = wsp->a;
	return a * sin(x) * cos(x);
}

double df(double x, void *wspolczynniki) {
	struct wsp_tryg *wsp = (struct wsp_tryg*) wspolczynniki;
	double a = wsp->a;
	return a * cos(2*x);
}

void fdf(double x, void *wspolczynniki, double *y, double *dy) {
	struct wsp_tryg *wsp = (struct wsp_tryg*) wspolczynniki;
	double a = wsp->a;
	*y = a * sin(x) * cos(x);
	*dy = a * cos(2*x);
}
