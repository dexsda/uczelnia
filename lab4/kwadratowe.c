struct wsp_kwadr {
	double a, b, c;
};

struct wsp_kwadr wsp = {1.0, 0.0, -5.0};
double x_min = 0.0, x_max = 5.0;
double root0, root = 5.0;

double f(double x, void *wspolczynniki) {
	struct wsp_kwadr *wsp = (struct wsp_kwadr*) wspolczynniki;
	double a = wsp->a;
	double b = wsp->b;
	double c = wsp->c;
	return (a * x + b) * x + c;
}

double df(double x, void *wspolczynniki) {
	struct wsp_kwadr *wsp = (struct wsp_kwadr*) wspolczynniki;
	double a = wsp->a;
	double b = wsp->b;
	return 2.0 * a * x  + b;
}

void fdf(double x, void *wspolczynniki, double *y, double *dy) {
	struct wsp_kwadr *wsp = (struct wsp_kwadr*) wspolczynniki;
	double a = wsp->a;
	double b = wsp->b;
	double c = wsp->c;
	*y = (a * x + b) * x + c;
	*dy = 2.0 * a * x  + b;
}
