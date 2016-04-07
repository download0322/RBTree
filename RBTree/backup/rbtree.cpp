#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"
#include <time.h>

int main(){
	RBTree T = NULL;
	Node   *x;
	int i = 0;
	srand((unsigned)time(NULL));
	for (i = 1; i <= 5; i++)
		rbInsert(&T, rand()%100);
	//rbInsert(&T, 90);
	//rbInsert(&T,80);
	//rbInsert(&T,40);
	// rbDelete(&T,rbSearch(T,130));
	// rbDelete(&T,rbSearch(T,90));
	//rbPrint(T);
	showRBTree(T);
	//printf("max=%d\n", rbMaxKey(T)->key);
	//printf("min=%d\n", rbMinKey(T)->key);
	//printf("search=%d\n", rbSearch(T, 80)->key);
	return 0;
}

/*��ȡ���׽ڵ�*/
static Node *Parent(Node *x) {
	return x->parent;
}

static Node *Left(Node *x) {
	return x->left;
}

static Node *Right(Node *x) {
	return x->right;
}

static void LeftRotate(RBTree *T, Node *x) {     //����ת�����xԭ����������y��ת��Ϊx�ĸ�ĸ
	if (x->right != nil)
	{
		//����size
		x->size = x->left->size + x->right->left->size + 1;
		x->right->size = x->size + x->right->right->size + 1;

		Node *y = Right(x);
		x->right = y->left;
		if (y->left != nil)
		{
			y->left->parent = x;
		}
		y->parent = x->parent;


		if (x->parent == nil)
		{
			*T = y;
		}
		else
		{
			if (x == Left(Parent(x)))
			{
				x->parent->left = y;
			}
			else
			{
				x->parent->right = y;
			}
		}
		y->left = x;
		x->parent = y;
	}
}



static void RightRotate(RBTree *T, Node *x)   //����ת�����xԭ����������y��ת��Ϊx�ĸ�ĸ
{
	if (x->left != nil)
	{
		//����size
		x->size = x->right->size + x->left->right->size + 1;
		x->left->size = x->size + x->left->left->size + 1;

		Node *y = Left(x);
		x->left = y->right;
		if (y->right != nil)
		{
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nil)
		{
			*T = y;
		}
		else
		{
			if (x == Left(Parent(x)))
			{
				x->parent->left = y;
			}
			else
			{
				x->parent->right = y;
			}
		}
		y->right = x;
		x->parent = y;
	}
}

/*����ʵ��Ҫɾ���Ľ��*/
static Node *RealDeleteNode(RBTree *T, Node *x) {
	Node *p = x->right;
	while (p->left != nil)
	{
		p = p->left;

	}
	return p;
}

void rbInsert(RBTree *T, int key) {
	if ((*T) == NULL) {
		*T = (Node*)malloc(sizeof(Node));
		//��ʼ��nil���
		nil = (Node*)malloc(sizeof(Node));
		nil->color = BLACK;
		nil->size = 0;
		//���ý���ָ��
		(*T)->parent = nil;
		(*T)->left = nil;
		(*T)->right = nil;
		//���ý������,key ��color
		(*T)->key = key;
		(*T)->color = BLACK;
		(*T)->size = 1;
	}
	else {
		Node *x = *T;
		Node *p = nil;
		while (x != nil) {
			p = x;
			p->size++;
			if (key>x->key)
				x = x->right;
			else if (key<x->key)
				x = x->left;
			else
				return;
		}
		x = (Node*)malloc(sizeof(Node));
		x->parent = p;
		x->left = nil;
		x->right = nil;

		x->key = key;
		x->color = RED;
		x->size = 1;
		if (key<p->key) {
			p->left = x;
		}
		else {
			p->right = x;
		}
		InsertFixup(T, x);
	}
}

/*�������*/
static void InsertFixup(RBTree *T, Node *z) {
	Node *y;
	while (Parent(z)->color == RED) {
		if (Parent(z) == Parent(Parent(z))->left) {
			y = Parent(Parent(z))->right;  //��ȡ�常���
			if (y->color == RED) {             //case 1  ����常���Ϊ��ɫ,��Ѹ��ڵ���常����ź�,�游����ź�,z���Ƶ��游���
				y->color = BLACK;
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				z = Parent(Parent(z));
			}
			else {
				if (z == Parent(z)->right) {   //case 2  ����常���Ϊ��ɫ,z�ҽ��,z����Ϊ���׽��,����תz���,��ʱ��Ϊcase3�����
					z = z->parent;
					LeftRotate(T, z);
				}
				z->parent->color = BLACK;   //case 3 �常���Ϊ��ɫ,��z������,z�ĸ������ź�,z���游�ź�,Ȼ����תz���游���
				Parent(Parent(z))->color = RED;
				RightRotate(T, Parent(Parent(z)));
			}
		}
		else {  //�Գ� ����ͬ��
			y = Parent(Parent(z))->left;
			if (y->color == RED) {
				y->color = BLACK;
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				z = Parent(Parent(z));
			}
			else {
				if (z == Parent(z)->left) {
					z = z->parent;
					RightRotate(T, z);
				}
				z->parent->color = BLACK;
				Parent(Parent(z))->color = RED;
				LeftRotate(T, Parent(Parent(z)));
			}
		}
	}
	(*T)->color = BLACK;
}

/**
*   ����ɾ������  ����ɾ���ڵ�Ϊz  ʵ��ɾ���ڵ�y xɾ��y��Ψһ�ӽڵ�(������)
*   1.�����һ������Ϊ��,ֱ��ɾ��  y=z
*   2.�����������Ϊ����,ɾ��������������ߵĽڵ�,���������Ϊ��,��ɾ�����Ƚڵ���Ϊ������������ y
*   3. if(y->left!=NULL) x=y->left else x= y->right;
*   4.��ԭ��y�ĸ�ĸ����Ϊx�ĸ�ĸ��Ϊy������ɾ��  ,x��y�����еķֲ�ȡ����y�ڸ����еķֲ�,���y����ڵ�,��xҲ����ڵ�,���y���ҽڵ���xҲ���ҽڵ�
*   5.���y!=z���y��ֵ����z
*   6.���yΪ��ɫ��,��Ҫ���е���,��Ϊ��ĳ��·��������һ����ɫ�ڵ�
*
*/

void rbDelete(RBTree *T, Node *z) {
	if (z == nil || z == NULL)
		return;
	Node *y;
	Node *x;
	if (z->left == nil || z->right == nil) {
		y = z;
	}
	else {
		y = RealDeleteNode(T, z);
	}
	//xָ��ʵ��ɾ�������ӽ��
	if (y->left != nil)
		x = y->left;
	else
		x = y->right;

	x->parent = y->parent;   //ɾ�����y
	if (y->parent == nil) {
		*T = x;
	}
	else {
		if (y == Parent(y)->left)
			y->parent->left = x;
		else
			y->parent->right = x;
	}
	if (y != z) {
		z->key = y->key;
	}
	//���ɾ���Ľ���Ǻ�ɫ,Υ��������5,Ҫ����ɾ������
	if (y->color == BLACK) {
		DeleteFixup(T, x);
	}
}



/*ɾ������*/
static void DeleteFixup(RBTree *T, Node *x) {
	while (x != (*T) && x->color == BLACK) {
		if (x == Parent(x)->left) {
			Node *w = Parent(x)->right;  //w Ϊx���ֵܽ��
			if (w->color == RED) {          //case 1 �ֵܽ��Ϊ��ɫ
				w->color = BLACK;
				x->parent->color = RED;
				LeftRotate(T, Parent(x));
				w = Parent(x)->right;
			}
			if (w == nil) break;
			if (Left(w)->color == BLACK && Right(w)->color == BLACK) {   //case2 �ֵܽ��������ӽ�㶼Ϊ��
				w->color = RED;
				x = Parent(x);
			}
			else if (w->right->color == BLACK) {    //case3 w��������Ϊ��ɫ,������Ϊ��ɫ
				w->color = RED;
				w->left->color = BLACK;
				RightRotate(T, w);
				w = Parent(x)->right;
			}
			w->color = x->parent->color;         //case 4 w��������Ϊ��ɫ
			x->parent->color = BLACK;
			w->right->color = BLACK;
			LeftRotate(T, Parent(x));
		}
		else {  //�Գ� ����ͬ��
			Node *w = Parent(x)->left;
			if (w->color == RED) {                 //case 1
				w->color = BLACK;
				x->parent->color = RED;
				RightRotate(T, Parent(x));
				w = Parent(x)->left;
			}
			if (w == nil) break;
			if (Left(w)->color == BLACK && Right(w)->color == BLACK) {  //case 2
				w->color = RED;
				x = Parent(x);
			}
			else if (w->left->color == BLACK) {                       //case 3
				w->color = RED;
				w->right->color = BLACK;
				LeftRotate(T, w);
				w = Parent(x)->left;
			}
			w->color = x->parent->color;
			x->parent->color = BLACK;
			w->left->color = BLACK;
			RightRotate(T, Parent(x));
		}
		x = *T;
	}
	x->color = BLACK;
}



/*����ĳ�����*/
Node *rbSearch(RBTree T, int key) {
	if (T != nil) {
		if (T->key<key)
			rbSearch(T->right, key);
		else if (T->key>key)
			rbSearch(T->left, key);
		else
			return T == nil ? NULL : T;
	}
}

/*��ӡ���*/
void rbPrint(RBTree T) {
	if (T != NULL && T != nil) {
		rbPrint(T->left);
		printf("%d(%s)\n", T->key, (T->color == RED) ? "��" : "��");
		rbPrint(T->right);
	}
}
/*������С��С���*/
Node *rbMinKey(RBTree T) {
	Node *x = T;
	Node *p = nil;
	while (x != nil) {
		p = x;
		x = x->left;
	}
	return p == nil ? NULL : p;
}

/*���������*/
Node *rbMaxKey(RBTree T) {
	Node *x = T;
	Node *p = nil;
	while (x != nil) {
		p = x;
		x = x->right;
	}
	return p == nil ? NULL : p;
}

/*���ӻ���ʾ*/
void showRBTree(RBTree T) {
	Node *x = T;

	if (x == nil) {
		for (int i = 0; i < deepth; i++) {
			printf("\t");
		}
		printf("nil\n");
		return;
	}

	deepth++;
	showRBTree(x->right);
	deepth--;

	for (int i = 0; i < deepth; i++) {
		printf("\t");
	}
	if (x->color == RED) {
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		WORD wOldColorAttrs;
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		GetConsoleScreenBufferInfo(h, &csbiInfo);
		wOldColorAttrs = csbiInfo.wAttributes;
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("%d %d\n", x->key, x->size);
		SetConsoleTextAttribute(h, wOldColorAttrs);
	}
	else {
		printf("%d %d\n", x->key, x->size);
	}

	deepth++;
	showRBTree(x->left);
	deepth--;
}

