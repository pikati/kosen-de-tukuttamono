//*���s���~�b�h�^�ŕ\������
#include <stdio.h>

void spira(int n){
	int j;
	int i;
	int input;//���͂��ꂽ�i��
	int count1 = 0;//*�̐������肷�邽�߂̕ϐ�
	int dansu = 1;//��ԏ����i�ڂƂ����Ƃ��̒i��

	input = n;
	//�X�y�[�X�̐��́A(���͂��ꂽ�i��-���݂̒i��)�ŋ��߂���
	//*�̐���1+(2*�i��-1)�ŋ��߂�ꂦ��B�܂�A�i�����P�����邲�Ƃɕ\�������鐔���Q���₹�΂���
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
		count1 += 2;//*���Q���������邽�߂̏���
		dansu++;
		//���̒i���ɐi��
	}
}

int main(void){
	int n;//�i��

	printf("�s���~�b�h�̒i������͂��Ă�������:");
	scanf("%d", &n);

	spira(n);

	rewind(stdin);
	getchar();

	return 0;
}