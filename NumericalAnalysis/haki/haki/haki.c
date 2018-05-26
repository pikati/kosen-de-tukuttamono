#include <stdio.h>
#include <math.h>

#define N 3

void hakidashi(double *a);
void zaideru(double * b);

int main(void) {
	double a[N][N + 1] = { { 2,2,6,24 },{ 3,5,13,52 },{ 5,8,24,93 } };
	double b[N][N + 1] = { {5,1,1,10},{1,4,1,12}, {2,1,3,13} };

	hakidashi(*a);
	zaideru(*b);

	return 0;
}

void hakidashi(double a[N][N + 1]) {
	for (int i = 0; i < N; i++) {
		double pivot = a[i][i];
		for (int j = i; j < N + 1; j++) {
			a[i][j] = a[i][j] / pivot;
		}
		for (int k = 0; k < N; k++) {
			if (k - i != 0) {
				double aik = a[k][i];
				for (int j = i; j < N + 1; j++) {
					a[k][j] = a[k][j] - aik*a[i][j];
				}
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		printf("%f\n", a[i][N]);
	}
}

void zaideru(double b[N][N + 1]) {
	double x[N];
	double y[N];
	int i;
	int j;
	int k;
	double q;
	int sum;
	double eps = 0.0001;

	for (i = 0; i < N; i++) {
		x[i] = 1.0;
		y[i] = x[i];
	}

	for (k = 1; k <= 30; k++) {
		for (i = 0; i < N; i++) {
			sum = 0;
			for (j = 0; j < N; j++) {
				if (i != j) {
					sum += b[i][j] * x[j];
				}
			}
			x[i] = (b[i][N] - sum) / b[i][i];
		}
		q = 0;

		for (i = 0; i < N; i++) {
			printf("xi%f yi%f\n", x[i], y[i]);
			q += fabs(x[i] - y[i]);
			y[i] = x[i];
		}

		//printf("",q)
		
		if (q < eps) {
			for (i = 0; i < N; i++) {
				printf("x %d = %f\n", i, x[i]);
				break;
			}
		}
	}
	if (k > 30) {
		puts("not");
	}
}