#include <stdio.h>
#include <math.h>

void TrapezoidalMethod();
void SimpsonMethod();
double func(double);

int main(void) {

	TrapezoidalMethod();
	SimpsonMethod();
	return 0;
}

void TrapezoidalMethod() {

	double a;
	double b;
	double h;
	double i;
	double k;
	double n;
	double s;
	double x;

	a = 0.0;
	b = 1.0;

	for (k = 1; k <= 10; k++) {
		s = 0;
		n = pow(2.0, k);
		h = (b - a) / n;
		for (i = 1; i < n; i++) {
			x = a + h * i;
			s += 2 * func(x);
		}
		s = 0.5 * (s + func(a) + func(b)) * h;
		printf("k=%f‚ÌŽžs=%f\n", k ,s);
	}
}

void SimpsonMethod() {

	double a;
	double b;
	double h;
	double k;
	double i;
	double n;
	double s;
	double s1;
	double s2;
	double x;

	a = 0.0;
	b = 1.0;

	for (k = 1; k <= 10; k++) {
		n = pow(2.0, k);
		h = (b - a) / (2 * n);
		s1 = 0;
		for (i = 1; i <= 2 * n - 1; i += 2) {

			x = a + h * i;
			s1 += 4 * func(x);
		}
		s2 = 0;
		for (i = 2; i <= 2 * n - 2; i += 2) {
			x = a + h * i;
			s2 += 2 * func(x);
		}
		s = (s1 + s2 + func(a) + func(b)) * h / 3;
		printf("k=%f‚ÌŽž%f\n", k, s);
	}
}

double func(double n) {
	return exp(n) * pow(n, 3);
}
