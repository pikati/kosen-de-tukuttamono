#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define max 1000                //�ő�J��Ԃ���
#define eps 0.15                //��������

double f(double x);
double df(double x);
void newton(void);

int main()
{
	newton();

	return 0;
}

void newton(void)
{
	int count;
	double a, newa;

	count = -1;

	a = 3.0;

	for (;;) {
		count++;

		newa = a - f(a) / df(a);

		if (fabs(newa - a) < eps) {
			break;
		}
		a = newa;

		if (count == max) {
			printf("�����Ȃ�");
			exit(1);
		}
	}
	printf("��:%f\ncount:%d\n",newa, count);
}

double f(double x)
{
	return x * x * x + x - 1;
}

double df(double x)
{
	return sin(x) + x*cos(x) + 1.0 / x;
}