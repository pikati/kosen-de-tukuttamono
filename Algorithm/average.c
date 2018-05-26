//ポインタを使い配列を用意して値を入力し平均を求める
#include <stdio.h>
#include <stdlib.h>

double aveof(const int a[], int n) {

	int i;
	double total = 0;

	for (i = 0; i < n; i++) {
		total += a[i];
	}

	return total/n;
}

int main(void) {

	int n;//要素数
	int *a;//配列の先頭要素へのポインタ
	int i;
	double avarage;

	printf("要素数を入力してください：");
	scanf("%d", &n);

	a = calloc(n, sizeof(int));

	if (a == NULL) {
		puts("記憶域の確保に失敗しました;");
	}
	else {
		printf("%d個の要素を入力してください\n", n);
		for (i = 0; i < n; i++) {
			printf("a[%d]：", i);
			scanf("%d", &a[i]);
		}
	}

	avarage = aveof(a, n);

	printf("平均値は%fです", avarage);

	rewind(stdin);
	getchar();

	return 0;
}
