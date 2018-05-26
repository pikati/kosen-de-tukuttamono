//�X�^�b�N�̎���
#include<stdio.h>
#include<stdlib.h>

#pragma warning (disable:4996)

# define N 5

typedef struct node {
	char data;			// �f�[�^
	struct node *next;	// �㑱�m�[�h�ւ̃|�C���^
} Node;

typedef struct {
	Node *head;			// �擪�m�[�h�ւ̃|�C���^
	Node *crnt;			// ���ڃm�[�h�ւ̃|�C���^
} List;

typedef struct {
	int max;			// �X�^�b�N�̍ő�l
	int num;			// �X�^�b�N�̌��݂̗�
	List stk;
} Stack;

/*---�X�^�b�N�Ƀf�[�^���v�b�V��---*/
void push(Stack *stk) {
	int i = 0;
	Node *ptr = stk->stk.head;
	while (ptr != NULL) {
		i++;
		ptr = ptr->next;
	}
	if (i < stk->max) {
		ptr = stk->stk.head;
		printf("�}������l�F");
		scanf("%d", &stk->num);
		stk->stk.crnt = (Node *)calloc(1, sizeof(Node));
		stk->stk.head = stk->stk.crnt;

		stk->stk.head->data = stk->num;
		stk->stk.head->next = ptr;
	}
	else
		printf("�X�^�b�N�����t�ł�\n");
}

/*---�X�^�b�N����f�[�^���|�b�v---*/
void pop(Stack *stk) {
	Node *ptr = stk->stk.head;

	if (ptr != NULL) {
		ptr = stk->stk.head;
		stk->stk.head = stk->stk.head->next;
		free(ptr);
	}
	else
		printf("�X�^�b�N����ł�\n");
}

/*---�S�f�[�^�̕\��---*/
void printList(Stack *stk) {
	Node *ptr = stk->stk.head;
	while (ptr != NULL) {
		printf("%d\n", ptr->data);
		ptr = ptr->next;
	}
}

/*---���C��---*/
int main(void) {
	Stack stackList;
	int comand, end_flag = 0;

	stackList.max = N;
	stackList.stk.head = NULL;// ������
	stackList.stk.crnt = NULL;

	while (end_flag == 0) {
		printf("0:�ǉ� | 1:��肾�� | 2:���X�g�̕\��");
		scanf("%d", &comand);
		switch (comand)
		{
		case 0:	push(&stackList);
			break;
		case 1: pop(&stackList);
			break;
		case 2: printList(&stackList);
			break;
		}

		printf("0:������ | 1:�I��:");
		scanf("%d", &end_flag);
	}
}