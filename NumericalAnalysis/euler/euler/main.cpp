#include <stdio.h>

void runge();
float func(float x, float y);

int main(void) {

	runge();
	return 0;
}

void runge() {

	float a;
	float b;
	float h;
	float n;
	float x;
	float y;
	float y0;
	float k1;
	float k2;
	float k3;
	float k4;

	puts("a");
	scanf("%f", &a);
	puts("b");
	scanf("%f", &b);
	puts("y0");
	scanf("%f", &y0);
	puts("n");
	scanf("%f", &n);

	h = (b - a) / n;
	x = a;
	y = y0;

	while (x < b) {
		k1 = func(x, y);
		k2 = func(x + h / 2.0, y + k1 * h / 2.0);
		k3 = func(x + h / 2.0, y + k2 * h / 2.0);
		k4 = func(x + h, y + k3 * h);

		y = y + h * (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6;
		x += h;
	}
	printf("y‚Ì’lF%f", y);
}

float func(float x, float y) {
	return x * y;
}