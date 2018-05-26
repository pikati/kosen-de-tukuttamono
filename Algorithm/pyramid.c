//*をピラミッド型で表示する
#include <stdio.h>

void spira(int n){
	int j;
	int i;
	int input;//入力された段数
	int count1 = 0;//*の数を決定するための変数
	int dansu = 1;//一番上を一段目としたときの段数

	input = n;
	//スペースの数は、(入力された段数-現在の段数)で求められる
	//*の数は1+(2*段数-1)で求められえる。つまり、段数が１つ増えるごとに表示させる数を２つ増やせばいい
	for (n; n > 0; n--){
		j = input - dansu;
		for (j; j > 0; j--){
			printf(" ");
		}
		for (i = 0; i <= count1; i++){
			printf("*");
		}
		j = input - dansu;
		for (j; j > 0; j--){
			printf(" ");
		}
		printf("\n");
		count1 += 2;//*を２つ多くさせるための処理
		dansu++;
		//次の段数に進む
	}
}

int main(void){
	int n;//段数

	printf("ピラミッドの段数を入力してください:");
	scanf("%d", &n);

	spira(n);

	rewind(stdin);
	getchar();

	return 0;
}