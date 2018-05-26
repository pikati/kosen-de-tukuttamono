//現在の日にちを入力してn日後の日にちを表示する
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int y;
	int m;
	int d;
} YMD;

int isleap(int year) {
	return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
}

YMD after(YMD x, int n) {
	int i;
	int count = n;

	while (count != 0) {//countは1日追加するごとに1づつ減っていく
		switch (x.m) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			for (i = 0; i < n; i++) {
				if (x.d == 31) {//31日だったら
					x.m++;//月を1足す
					x.d = 0;//日は0にする
					if (x.m == 13) {//月を足した結果13になったら
						x.y++;//年を1追加して
						x.m -= 12;//1月にする
					}
				}
				x.d++;//日を1追加
				count--;//残り繰り返し回数を1減らす
			}
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			for (i = 0; i < n; i++) {
				if (x.d == 30) {//30日だったら
					x.m++;//月を1足す
					x.d = 0;//日は0にする
					if (x.m == 13) {//月を足した結果13になったら
						x.y++;//年を1追加して
						x.m -= 12;//1月にする
					}
				}
				x.d++;//日を1追加
				count--;//残り繰り返し回数を1減らす
			}
			break;
		case 2:
			if (isleap(x.y) == 0) {
				for (i = 0; i < n; i++) {
					if (x.d == 28) {//28日だったら
						x.m++;//月を1足す
						x.d = 0;//日は0にする
						if (x.m == 13) {//月を足した結果13になったら
							x.y++;//年を1追加して
							x.m -= 12;//1月にする
						}
					}
					x.d++;//日を1追加
					count--;//残り繰り返し回数を1減らす
				}
			}
			else {
				for (i = 0; i < n; i++) {
					if (x.d == 29) {//29日だったら
						x.m++;//月を1足す
						x.d = 0;//日は0にする
						if (x.m == 13) {//月を足した結果13になったら
							x.y++;//年を1追加して
							x.m -= 12;//1月にする
						}
					}
					x.d++;//日を1追加
					count--;//残り繰り返し回数を1減らす
				}
			}
			break;
		}
	}

	return x;
}

int main(void) {

	YMD ymd;
	int check = 0;//日が正常に入力されているかをチェックする変数
	int n;

	printf("現在の日にちを入力してください\n年：");
	scanf("%d", &ymd.y);

	//1～12が入力されない限り繰り返す
	do {
		printf("月：");
		scanf("%d", &ymd.m);
		if (ymd.m < 1 || ymd.m > 12) {
			puts("1～12の範囲で入力してください");
		}
	} while (ymd.m < 1 || ymd.m > 12);

	//月に応じた正しい日が入力されない限り繰り返す
	do {
		printf("日：");
		scanf("%d", &ymd.d);
		if (ymd.m == 2) {
			if (isleap(ymd.y) == 0 && ymd.d > 28) {
				puts("2月は28日までです");
			}
			else if (isleap(ymd.y) == 1 && ymd.d > 29) {
				puts("うるう年は29日まであります");
			}
			else {
				check = 1;
			}
		}
		else if (ymd.m == 4 || ymd.m == 6 || ymd.m == 9 || ymd.m == 11) {
			if (ymd.d > 30) {
				puts("30日までです");
			}
			else {
				check = 1;
			}
		}
		else {
			if (ymd.d > 31) {
				puts("31日までです");
			}
			else {
				check = 1;
			}
		}
	} while (check != 1);


	printf("今日は%d年%d月%d日ですね", ymd.y, ymd.m, ymd.d);
	printf("何日後の日にちが知りたいですか？\n");
	scanf("%d", &n);

	ymd = after(ymd, n);

	printf("%d日後は%d年%d月%d日です\n", n, ymd.y, ymd.m, ymd.d);

	rewind(stdin);
	getchar();

	return 0;
}