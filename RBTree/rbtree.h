#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> 

/**
* �����������
* 1.ÿ�����Ҫô��ɫ,Ҫô��ɫ
* 2.������Ǻ�ɫ��
* 3.ÿ��Ҷ�ӽ�㶼�Ǻ�ɫ(nil���)
* 4.���һ������Ϊ��ɫ,��ô�����ӽ�㶼�Ǻ�ɫ   (����������������ɫ���)
* 5.����һ��㵽Ҷ�ӽ��ÿ��·���Ϻ�ɫ���ĸ��������
*/




/*���������ɫ*/
typedef enum Color{
	RED = 1,
	BLACK = 0
}Color;
typedef struct Node{
	struct Node *parent;  //���ڵ�
	struct Node *left;    //������
	struct Node *right;   //������
	int          key;     //key
	/*data ��*/
	Color        color;   //�ڵ���ɫ
	int size;            //�ڽ����
}Node, *RBTree;

/*ȫ�ֱ���*/
int deepth = 0;

/*����һ�����*/
void  rbInsert(RBTree *T, int key);
/*���ҽ��*/
Node *rbSearch(RBTree T, int key);
/*ɾ�����*/
void  rbDelete(RBTree *T, Node *z);
/*��ӡ�����*/
void  rbPrint(RBTree T);
/*���ӻ���ʾ*/
void showRBTree(RBTree T);
/*���ҵ�kС��Ԫ��*/
int Select(RBTree T, int i);
/*����Ԫ��x����*/
int Rank(RBTree T, RBTree x);



/*�������*/
static void InsertFixup(RBTree *T, Node *z);
/*ɾ������*/
static void DeleteFixup(RBTree *T, Node *x);
static Node *Parent(Node *x);
static Node *Left(Node *x);
static Node *Right(Node *x);
static void LeftRotate(RBTree *T, Node *x);
static void RightRotate(RBTree *T, Node *x);
static Node *RealDeleteNode(RBTree *T, Node *x);

/*����ڵ��ָ��*/
static Node *nil = NULL;