#include <stdio.h>
#include <math.h>

void PincerAttackMethod();
double func(double);

int main() {
	PincerAttackMethod();
	return 0;
}

void PincerAttackMethod() {
	double a;
	double b;
	double c;
	double fa;
	double fb;
	double fc;
	double eps;
	int count;

	count = -1;

	a = 1.0;
	b = -1.0;
	eps = 0.05;

	do {
		fa = func(a);
		fb = func(b);
		c = (a*fb - b*fa) / (fb - fa);
		fc = func(c);
		if (fc*fb < 0) {
			a = c;
		}
		else {
			b = c;
		}
		count++;
	} while (fabs(fc) > eps);

	printf("fabsfc:%f\n", fabs(fc));
	printf("count:%d\n", count);
	printf("%f\n", c);
}

double func(double a) {
	return a * a * a + a - 1;
}