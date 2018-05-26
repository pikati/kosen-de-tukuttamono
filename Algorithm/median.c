//3つの数値の中央値をいろんなパターンで教える
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
	int a, b, c;//入力する3つの値

	printf("1番目の数値：");
	scanf("%d", &a);
	printf("2番目の数値：");
	scanf("%d", &b);
	printf("3番目の数値：");
	scanf("%d", &c);

	printf("%d,%d,%dの中央値は%dです\n", a, b, c, med(a, b, c));
	printf("%d,%d,%dの中央値は%dです\n", a, b, b, med(a, b, b));
	printf("%d,%d,%dの中央値は%dです\n", a, c, b, med(a, c, b));
	printf("%d,%d,%dの中央値は%dです\n", a, b, a, med(a, b, a));
	printf("%d,%d,%dの中央値は%dです\n", b, c, a, med(b, c, a));
	printf("%d,%d,%dの中央値は%dです\n", a, a, b, med(a, a, b));
	printf("%d,%d,%dの中央値は%dです\n", a, a, a, med(a, a, a));
	printf("%d,%d,%dの中央値は%dです\n", b, b, a, med(b, b, a));
	printf("%d,%d,%dの中央値は%dです\n", b, a, c, med(b, a, c));
	printf("%d,%d,%dの中央値は%dです\n", b, a, b, med(b, a, b));
	printf("%d,%d,%dの中央値は%dです\n", c, a, b, med(c, a, b));
	printf("%d,%d,%dの中央値は%dです\n", b, c, c, med(b, c, c));
	printf("%d,%d,%dの中央値は%dです\n", c, b, a, med(c, b, a));

	rewind(stdin);
	getchar();

	return 0;
}