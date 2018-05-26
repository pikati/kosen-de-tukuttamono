//年月日を入力しその日がその年の何日目かを判定
#include <stdio.h>
#include <stdlib.h>

int mdays[][12] = {
	{31,28,31,30,31,30,31,31,30,31,30,31},
	{31,29,31,30,31,30,31,31,30,31,30,31},
};

int isleap(int year) {
	return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
}

int daysofyear(int y, int m, int d) {
	int n = m - 1;
	while (n >= 0) {
		n--;
		d += mdays[isleap(y)][n];
		return d;
	}
}

int main(void) {

	int year;
	int month;
	int day;
	int retry;

	do {
		printf("年：");
		scanf("%d", &year);
		printf("月：");
		scanf("%d", &month);
		printf("日：");
		scanf("%d", &day);

		printf("年内で%d日目です\n", daysofyear(year, month, day));

		printf("もう1度しますか（1…はい/0…いいえ）：");
		scanf("%d", &retry);
	} while (retry == 1);

	rewind(stdin);
	getchar();

	return 0;
}