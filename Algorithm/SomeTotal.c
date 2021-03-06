//2つの値の間の総和を求める
#include <stdio.h>

void sumof(int a, int b){

	int max;
	int min;
	int sum = 0;

	if (a < b){
		max = b;
		min = a;
		for (min; min <= max; min++){
			sum += min;
		}
	}
	else{
		max = a;
		min = b;
		for (min; min <= max; min++){
			sum += min;
		}
	}
	printf("%dと%dの間の全整数の和は%dです", a, b, sum);
}

int main(void){

	int a, b;

	printf("2つの値を入力してください\n１つ目：");
	scanf("\n%d", &a);

	printf("２つ目：");
	scanf("\n%d", &b);

	sumof(a, b);

	rewind(stdin);
	getchar();

	return 0;
}