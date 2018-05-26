//�N��������͂����̓������̔N�̉����ڂ��𔻒�
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
		printf("�N�F");
		scanf("%d", &year);
		printf("���F");
		scanf("%d", &month);
		printf("���F");
		scanf("%d", &day);

		printf("�N����%d���ڂł�\n", daysofyear(year, month, day));

		printf("����1�x���܂����i1�c�͂�/0�c�������j�F");
		scanf("%d", &retry);
	} while (retry == 1);

	rewind(stdin);
	getchar();

	return 0;
}