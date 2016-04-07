#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> 

/**
* 红黑树的性质
* 1.每个结点要么红色,要么黑色
* 2.根结点是黑色的
* 3.每个叶子结点都是黑色(nil结点)
* 4.如果一个几点为红色,那么两个子结点都是黑色   (不能有连续两个红色结点)
* 5.任意一结点到叶子结点每条路径上黑色结点的个数都相等
*/




/*定义结点的颜色*/
typedef enum Color{
	RED = 1,
	BLACK = 0
}Color;
typedef struct Node{
	struct Node *parent;  //父节点
	struct Node *left;    //左子树
	struct Node *right;   //右子树
	int          key;     //key
	/*data 域*/
	Color        color;   //节点颜色
	int size;            //内结点数
}Node, *RBTree;

/*全局变量*/
int deepth = 0;

/*插入一个结点*/
void  rbInsert(RBTree *T, int key);
/*查找结点*/
Node *rbSearch(RBTree T, int key);
/*删除结点*/
void  rbDelete(RBTree *T, Node *z);
/*打印红黑树*/
void  rbPrint(RBTree T);
/*可视化显示*/
void showRBTree(RBTree T);
/*查找第k小的元素*/
int Select(RBTree T, int i);
/*查找元素x的秩*/
int Rank(RBTree T, RBTree x);



/*插入调整*/
static void InsertFixup(RBTree *T, Node *z);
/*删除调整*/
static void DeleteFixup(RBTree *T, Node *x);
static Node *Parent(Node *x);
static Node *Left(Node *x);
static Node *Right(Node *x);
static void LeftRotate(RBTree *T, Node *x);
static void RightRotate(RBTree *T, Node *x);
static Node *RealDeleteNode(RBTree *T, Node *x);

/*定义节点空指针*/
static Node *nil = NULL;