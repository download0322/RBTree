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

/**
*  ����ת����  ���赱ǰ���Ϊx
*  1.����Ϊ������������
*      x->parent = x->right;
*  2.���������Ʊ�Ϊ���׽��,���ݽ��ķֲ���ȷ��,������Ϊ���׺�,������游���ķֲ�
*      if(x==x->parent->parent->left){
*          x->parent->parent->left = x->right;
*      }else{
*          x->parent->parent->right = x->right;
*      }
*      x->right->parent = x->parent;
*  3.��������������Ϊ��ǰ�����ҽ��
*      x->right = x->right->left;
*
*         5                               7
*          \         ���5����ת         /  \
*           7       -------------->      5   8
*          / \                            \
*          6  8                            6
*/


/**
*  ����ת����  ���赱ǰ���Ϊx
*  1.����Ϊ���������ҽ��
*      x->parent = x->left;
*  2.���������Ʊ�Ϊ���׽��,���ݽ��ķֲ���ȷ������������游���ķֲ�
*      if(x==x->parent->left){
*          x->parent->parent->left = x->left;
*      }else{
*          x->parent->parent->right = x->left;
*      }
*      x->left->parent = x->parent;
*  3.���������ҽ��,��Ϊ��ǰ��������
*      x->left = x->left->right;
*
*      4                                 2
*     /           �ڵ�4����ת           / \
*     2          ------------->         1  4
*    / \                                   /
*    1  3                                 3
*/


/**
*   ���Ĳ������
*   1.�ڵ�����׶˲���,�ҽڵ��Ǻ�ɫ�� (�������ڵ�x)
*   2.�����������ڵ��Ǻ�ɫ��,��Ҫ���е���
*     1.x�ĸ��ڵ����游�ڵ��������
*      case1 :�常�ڵ�y�Ǻ�ɫ
*              1.��y��x�ĸ���ͬʱ��Ϊ��ɫ
*              2.Ȼ���x���游��Ϊ��ɫ
*              3.��x����Ϊ�游�ڵ�
*      case2:�常�ڵ��Ǻ�ɫ,x���ҽڵ�
*              1.��x��Ϊx��parent
*              2.����תx
*      case3:�常�ڵ��Ǻ�ɫ,x����ڵ�
*              1.��x��parent���
*              2.x���游���
*              3.����תx���游�ڵ�
*     2.x�ĸ��ڵ����游�ڵ��������(����ͬ��)
*   3.�������Ϊ���ڵ�,�򽫸��ڵ���
*
*/

/**
*   ����ɾ������  ����ɾ���ڵ�Ϊz  ʵ��ɾ���ڵ�y xɾ��y��Ψһ�ӽڵ�(������)
*   1.�����һ������Ϊ��,ֱ��ɾ��  y=z
*   2.�����������Ϊ����,ɾ��������������ߵĽڵ�,���������Ϊ��,��ɾ�����Ƚڵ���Ϊ������������ y
*   3. if(y->left!=NULL) x=y->left else x= y->right;
*   4.��ԭ��y�ĸ�ĸ����Ϊx�ĸ�ĸ��Ϊy������ɾ��  ,x��y�����еķֲ�ȡ����y�ڸ����еķֲ�,���y����ڵ�,��xҲ����ڵ�,���y���ҽڵ���xҲ���ҽڵ�
*   5.���y!=z���y��ֵ����z
*   6.���yΪ��ɫ��,��Ҫ���е���,��Ϊ��ĳ��·��������һ����ɫ�ڵ�
*
*   ɾ��������������:
*      1.ɾ���ĵ�����ѭ����,������ x��Ϊ���ڵ���x��һ����ɫ�ڵ�,���x��һ����ɫ�ڵ���,ֱ��Ⱦ�켴��
*      2.���������
*          x�������� ����x���ֵܽڵ�Ϊw
*              case 1: wΪ��ɫ
*                      1.w�ź�
*                      2.x��parent���
*                      3.����תx��parent
*                      4.�����ֵܽڵ� w = x->parent->right;
*              case 2: w��ɫ �������������Ǻ�ɫ
*                      1.w �ź�
*                      2.x��Ϊx��parent
*              case 3: w��ɫ w���������Ǻ�ɫ w���������Ǻ�ɫ
*                      1.w�ź�
*                      2.w�������ź�
*                      3.����תw
*                      4.�����ֵܽڵ�   w=x->parent->right;
*              case 4: w���������Ǻ�ɫ
*                      1.w��ɫΪx���׵���ɫ
*                      2.x��parent�ź�
*                      3.w���������ź�
*                      4.����תx��parent
*              ������� x=T  �˳�
*          x�������� (����ͬ��)
*      3.���ø��ڵ����ɫ,�����п��ܸı��˸��ڵ����ɫ
*
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
	int			 size;
}Node, *RBTree;

/*ȫ�ֱ���*/
int deepth = 0;

/*����һ�����*/
void  rbInsert(RBTree *T, int key);
/*���ҽ��*/
Node *rbSearch(RBTree T, int key);
/*ɾ�����*/
void  rbDelete(RBTree *T, Node *z);
/*������С�Ľ��*/
Node *rbMinKey(RBTree T);
/*�������Ľ��*/
Node *rbMaxKey(RBTree T);
/*��ӡ�����*/
void  rbPrint(RBTree T);
/*���ӻ���ʾ*/
void showRBTree(RBTree T);
/*���ҵ�kС����*/
int OS_Select(RBTree T, int k);


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