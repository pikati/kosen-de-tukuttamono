//3�̐��l�̒����l�������ȃp�^�[���ŋ�����
#include <stdio.h>

int med(a, b, c){

	if (a >= b){
		if (b >= c){
			return b;
		}
		else if(a <= c){
			return a;
		}
		else{
			return c;
		}
	}
	else if (a > c){
		return a;
	}
	else if (b > c){
		return c;
	}
	else{
		return b;
	}

}
int main(void){
	int a, b, c;//���͂���3�̒l

	printf("1�Ԗڂ̐��l�F");
	scanf("%d", &a);
	printf("2�Ԗڂ̐��l�F");
	scanf("%d", &b);
	printf("3�Ԗڂ̐��l�F");
	scanf("%d", &c);

	printf("%d,%d,%d�̒����l��%d�ł�\n", a, b, c, med(a, b, c));
	printf("%d,%d,%d�̒����l��%d�ł�\n", a, b, b, med(a, b, b));
	printf("%d,%d,%d�̒����l��%d�ł�\n", a, c, b, med(a, c, b));
	printf("%d,%d,%d�̒����l��%d�ł�\n", a, b, a, med(a, b, a));
	printf("%d,%d,%d�̒����l��%d�ł�\n", b, c, a, med(b, c, a));
	printf("%d,%d,%d�̒����l��%d�ł�\n", a, a, b, med(a, a, b));
	printf("%d,%d,%d�̒����l��%d�ł�\n", a, a, a, med(a, a, a));
	printf("%d,%d,%d�̒����l��%d�ł�\n", b, b, a, med(b, b, a));
	printf("%d,%d,%d�̒����l��%d�ł�\n", b, a, c, med(b, a, c));
	printf("%d,%d,%d�̒����l��%d�ł�\n", b, a, b, med(b, a, b));
	printf("%d,%d,%d�̒����l��%d�ł�\n", c, a, b, med(c, a, b));
	printf("%d,%d,%d�̒����l��%d�ł�\n", b, c, c, med(b, c, c));
	printf("%d,%d,%d�̒����l��%d�ł�\n", c, b, a, med(c, b, a));

	rewind(stdin);
	getchar();

	return 0;
}