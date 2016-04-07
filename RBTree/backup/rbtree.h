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

/**
*  左旋转规则  假设当前结点为x
*  1.结点变为右子树的左结点
*      x->parent = x->right;
*  2.右子树上移变为父亲结点,根据结点的分布来确定,右树变为父亲后,相对于祖父结点的分布
*      if(x==x->parent->parent->left){
*          x->parent->parent->left = x->right;
*      }else{
*          x->parent->parent->right = x->right;
*      }
*      x->right->parent = x->parent;
*  3.右子树的左结点作为当前结点的右结点
*      x->right = x->right->left;
*
*         5                               7
*          \         结点5左旋转         /  \
*           7       -------------->      5   8
*          / \                            \
*          6  8                            6
*/


/**
*  右旋转规则  假设当前结点为x
*  1.结点变为左子树的右结点
*      x->parent = x->left;
*  2.左子树上移变为父亲结点,根据结点的分布来确定父亲相对于祖父结点的分布
*      if(x==x->parent->left){
*          x->parent->parent->left = x->left;
*      }else{
*          x->parent->parent->right = x->left;
*      }
*      x->left->parent = x->parent;
*  3.左子树的右结点,作为当前结点的左结点
*      x->left = x->left->right;
*
*      4                                 2
*     /           节点4右旋转           / \
*     2          ------------->         1  4
*    / \                                   /
*    1  3                                 3
*/


/**
*   结点的插入规则
*   1.节点在最底端插入,且节点是红色的 (假设插入节点x)
*   2.插入后如果父节点是红色的,则要进行调整
*     1.x的父节点是祖父节点的左子树
*      case1 :叔父节点y是红色
*              1.将y与x的父亲同时着为黑色
*              2.然后把x的祖父变为红色
*              3.把x上移为祖父节点
*      case2:叔父节点是黑色,x是右节点
*              1.将x变为x的parent
*              2.左旋转x
*      case3:叔父节点是黑色,x是左节点
*              1.将x的parent变黑
*              2.x的祖父变红
*              3.右旋转x的祖父节点
*     2.x的父节点是祖父节点的右子树(调整同上)
*   3.如果上升为根节点,则将根节点变黑
*
*/

/**
*   结点的删除规则  假设删除节点为z  实际删除节点y x删除y的唯一子节点(先左后后)
*   1.如果有一个子树为空,直接删除  y=z
*   2.如果右子树不为空则,删除右子树中最左边的节点,如果右子树为空,则删除祖先节点中为左子树的祖先 y
*   3. if(y->left!=NULL) x=y->left else x= y->right;
*   4.将原来y的父母设置为x的父母因为y即将被删除  ,x在y父亲中的分布取决与y在父亲中的分布,如果y是左节点,则x也是左节点,如果y是右节点则x也是右节点
*   5.如果y!=z则把y的值换给z
*   6.如果y为黑色则,则要进行调整,因为在某条路径上少了一个黑色节点
*
*   删除调整规则如下:
*      1.删除的调整是循环的,条件是 x不为根节点且x是一个黑色节点,如果x是一个黑色节点则,直接染红即可
*      2.分两种情况
*          x是左子树 假设x的兄弟节点为w
*              case 1: w为红色
*                      1.w着黑
*                      2.x的parent变红
*                      3.左旋转x的parent
*                      4.重置兄弟节点 w = x->parent->right;
*              case 2: w黑色 且两个子树都是黑色
*                      1.w 着黑
*                      2.x变为x的parent
*              case 3: w黑色 w的左子树是红色 w的右子树是黑色
*                      1.w着红
*                      2.w的左子着黑
*                      3.右旋转w
*                      4.重置兄弟节点   w=x->parent->right;
*              case 4: w的右子树是红色
*                      1.w着色为x父亲的颜色
*                      2.x的parent着黑
*                      3.w的右子树着黑
*                      4.左旋转x的parent
*              调整完成 x=T  退出
*          x是右子树 (过程同上)
*      3.设置根节点的颜色,过程有可能改变了根节点的颜色
*
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
	int			 size;
}Node, *RBTree;

/*全局变量*/
int deepth = 0;

/*插入一个结点*/
void  rbInsert(RBTree *T, int key);
/*查找结点*/
Node *rbSearch(RBTree T, int key);
/*删除结点*/
void  rbDelete(RBTree *T, Node *z);
/*查找最小的结点*/
Node *rbMinKey(RBTree T);
/*查找最大的结点*/
Node *rbMaxKey(RBTree T);
/*打印红黑树*/
void  rbPrint(RBTree T);
/*可视化显示*/
void showRBTree(RBTree T);
/*查找第k小的数*/
int OS_Select(RBTree T, int k);


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