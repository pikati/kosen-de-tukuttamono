//�|�C���^���g���z���p�ӂ��Ēl����͂����ς����߂�
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

	int n;//�v�f��
	int *a;//�z��̐擪�v�f�ւ̃|�C���^
	int i;
	double avarage;

	printf("�v�f������͂��Ă��������F");
	scanf("%d", &n);

	a = calloc(n, sizeof(int));

	if (a == NULL) {
		puts("�L����̊m�ۂɎ��s���܂���;");
	}
	else {
		printf("%d�̗v�f����͂��Ă�������\n", n);
		for (i = 0; i < n; i++) {
			printf("a[%d]�F", i);
			scanf("%d", &a[i]);
		}
	}

	avarage = aveof(a, n);

	printf("���ϒl��%f�ł�", avarage);

	rewind(stdin);
	getchar();

	return 0;
}
