//2�̒l�̊Ԃ̑��a�����߂�
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
	printf("%d��%d�̊Ԃ̑S�����̘a��%d�ł�", a, b, sum);
}

int main(void){

	int a, b;

	printf("2�̒l����͂��Ă�������\n�P�ځF");
	scanf("\n%d", &a);

	printf("�Q�ځF");
	scanf("\n%d", &b);

	sumof(a, b);

	rewind(stdin);
	getchar();

	return 0;
}