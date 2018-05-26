//スタックの実装
#include<stdio.h>
#include<stdlib.h>

#pragma warning (disable:4996)

# define N 5

typedef struct node {
	char data;			// データ
	struct node *next;	// 後続ノードへのポインタ
} Node;

typedef struct {
	Node *head;			// 先頭ノードへのポインタ
	Node *crnt;			// 着目ノードへのポインタ
} List;

typedef struct {
	int max;			// スタックの最大値
	int num;			// スタックの現在の量
	List stk;
} Stack;

/*---スタックにデータをプッシュ---*/
void push(Stack *stk) {
	int i = 0;
	Node *ptr = stk->stk.head;
	while (ptr != NULL) {
		i++;
		ptr = ptr->next;
	}
	if (i < stk->max) {
		ptr = stk->stk.head;
		printf("挿入する値：");
		scanf("%d", &stk->num);
		stk->stk.crnt = (Node *)calloc(1, sizeof(Node));
		stk->stk.head = stk->stk.crnt;

		stk->stk.head->data = stk->num;
		stk->stk.head->next = ptr;
	}
	else
		printf("スタックが満杯です\n");
}

/*---スタックからデータをポップ---*/
void pop(Stack *stk) {
	Node *ptr = stk->stk.head;

	if (ptr != NULL) {
		ptr = stk->stk.head;
		stk->stk.head = stk->stk.head->next;
		free(ptr);
	}
	else
		printf("スタックが空です\n");
}

/*---全データの表示---*/
void printList(Stack *stk) {
	Node *ptr = stk->stk.head;
	while (ptr != NULL) {
		printf("%d\n", ptr->data);
		ptr = ptr->next;
	}
}

/*---メイン---*/
int main(void) {
	Stack stackList;
	int comand, end_flag = 0;

	stackList.max = N;
	stackList.stk.head = NULL;// 初期化
	stackList.stk.crnt = NULL;

	while (end_flag == 0) {
		printf("0:追加 | 1:取りだし | 2:リストの表示");
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

		printf("0:続ける | 1:終了:");
		scanf("%d", &end_flag);
	}
}