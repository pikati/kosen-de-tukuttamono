//���݂̓��ɂ�����͂���n����̓��ɂ���\������
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

	while (count != 0) {//count��1���ǉ����邲�Ƃ�1�Â����Ă���
		switch (x.m) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			for (i = 0; i < n; i++) {
				if (x.d == 31) {//31����������
					x.m++;//����1����
					x.d = 0;//����0�ɂ���
					if (x.m == 13) {//���𑫂�������13�ɂȂ�����
						x.y++;//�N��1�ǉ�����
						x.m -= 12;//1���ɂ���
					}
				}
				x.d++;//����1�ǉ�
				count--;//�c��J��Ԃ��񐔂�1���炷
			}
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			for (i = 0; i < n; i++) {
				if (x.d == 30) {//30����������
					x.m++;//����1����
					x.d = 0;//����0�ɂ���
					if (x.m == 13) {//���𑫂�������13�ɂȂ�����
						x.y++;//�N��1�ǉ�����
						x.m -= 12;//1���ɂ���
					}
				}
				x.d++;//����1�ǉ�
				count--;//�c��J��Ԃ��񐔂�1���炷
			}
			break;
		case 2:
			if (isleap(x.y) == 0) {
				for (i = 0; i < n; i++) {
					if (x.d == 28) {//28����������
						x.m++;//����1����
						x.d = 0;//����0�ɂ���
						if (x.m == 13) {//���𑫂�������13�ɂȂ�����
							x.y++;//�N��1�ǉ�����
							x.m -= 12;//1���ɂ���
						}
					}
					x.d++;//����1�ǉ�
					count--;//�c��J��Ԃ��񐔂�1���炷
				}
			}
			else {
				for (i = 0; i < n; i++) {
					if (x.d == 29) {//29����������
						x.m++;//����1����
						x.d = 0;//����0�ɂ���
						if (x.m == 13) {//���𑫂�������13�ɂȂ�����
							x.y++;//�N��1�ǉ�����
							x.m -= 12;//1���ɂ���
						}
					}
					x.d++;//����1�ǉ�
					count--;//�c��J��Ԃ��񐔂�1���炷
				}
			}
			break;
		}
	}

	return x;
}

int main(void) {

	YMD ymd;
	int check = 0;//��������ɓ��͂���Ă��邩���`�F�b�N����ϐ�
	int n;

	printf("���݂̓��ɂ�����͂��Ă�������\n�N�F");
	scanf("%d", &ymd.y);

	//1�`12�����͂���Ȃ�����J��Ԃ�
	do {
		printf("���F");
		scanf("%d", &ymd.m);
		if (ymd.m < 1 || ymd.m > 12) {
			puts("1�`12�͈̔͂œ��͂��Ă�������");
		}
	} while (ymd.m < 1 || ymd.m > 12);

	//���ɉ������������������͂���Ȃ�����J��Ԃ�
	do {
		printf("���F");
		scanf("%d", &ymd.d);
		if (ymd.m == 2) {
			if (isleap(ymd.y) == 0 && ymd.d > 28) {
				puts("2����28���܂łł�");
			}
			else if (isleap(ymd.y) == 1 && ymd.d > 29) {
				puts("���邤�N��29���܂ł���܂�");
			}
			else {
				check = 1;
			}
		}
		else if (ymd.m == 4 || ymd.m == 6 || ymd.m == 9 || ymd.m == 11) {
			if (ymd.d > 30) {
				puts("30���܂łł�");
			}
			else {
				check = 1;
			}
		}
		else {
			if (ymd.d > 31) {
				puts("31���܂łł�");
			}
			else {
				check = 1;
			}
		}
	} while (check != 1);


	printf("������%d�N%d��%d���ł���", ymd.y, ymd.m, ymd.d);
	printf("������̓��ɂ����m�肽���ł����H\n");
	scanf("%d", &n);

	ymd = after(ymd, n);

	printf("%d�����%d�N%d��%d���ł�\n", n, ymd.y, ymd.m, ymd.d);

	rewind(stdin);
	getchar();

	return 0;
}