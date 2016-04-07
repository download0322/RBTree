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

/*获取父亲节点*/
static Node *Parent(Node *x) {
	return x->parent;
}

static Node *Left(Node *x) {
	return x->left;
}

static Node *Right(Node *x) {
	return x->right;
}

static void LeftRotate(RBTree *T, Node *x) {     //左旋转：结点x原来的右子树y旋转成为x的父母
	if (x->right != nil)
	{
		//调整size
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



static void RightRotate(RBTree *T, Node *x)   //右旋转：结点x原来的左子树y旋转成为x的父母
{
	if (x->left != nil)
	{
		//调整size
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

/*查找实际要删除的结点*/
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
		//初始化nil结点
		nil = (Node*)malloc(sizeof(Node));
		nil->color = BLACK;
		nil->size = 0;
		//设置结点的指向
		(*T)->parent = nil;
		(*T)->left = nil;
		(*T)->right = nil;
		//设置结点属性,key 和color
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

/*插入调整*/
static void InsertFixup(RBTree *T, Node *z) {
	Node *y;
	while (Parent(z)->color == RED) {
		if (Parent(z) == Parent(Parent(z))->left) {
			y = Parent(Parent(z))->right;  //获取叔父结点
			if (y->color == RED) {             //case 1  如果叔父结点为红色,则把父节点和叔父结点着黑,祖父结点着红,z上移到祖父结点
				y->color = BLACK;
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				z = Parent(Parent(z));
			}
			else {
				if (z == Parent(z)->right) {   //case 2  如果叔父结点为黑色,z右结点,z上移为父亲结点,左旋转z结点,此时变为case3的情况
					z = z->parent;
					LeftRotate(T, z);
				}
				z->parent->color = BLACK;   //case 3 叔父结点为黑色,且z的左结点,z的父亲着着黑,z的祖父着红,然后旋转z的祖父结点
				Parent(Parent(z))->color = RED;
				RightRotate(T, Parent(Parent(z)));
			}
		}
		else {  //对称 调整同上
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
*   结点的删除规则  假设删除节点为z  实际删除节点y x删除y的唯一子节点(先左后后)
*   1.如果有一个子树为空,直接删除  y=z
*   2.如果右子树不为空则,删除右子树中最左边的节点,如果右子树为空,则删除祖先节点中为左子树的祖先 y
*   3. if(y->left!=NULL) x=y->left else x= y->right;
*   4.将原来y的父母设置为x的父母因为y即将被删除  ,x在y父亲中的分布取决与y在父亲中的分布,如果y是左节点,则x也是左节点,如果y是右节点则x也是右节点
*   5.如果y!=z则把y的值换给z
*   6.如果y为黑色则,则要进行调整,因为在某条路径上少了一个黑色节点
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
	//x指向实际删除结点的子结点
	if (y->left != nil)
		x = y->left;
	else
		x = y->right;

	x->parent = y->parent;   //删除结点y
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
	//如果删除的结点是黑色,违法了性质5,要进行删除调整
	if (y->color == BLACK) {
		DeleteFixup(T, x);
	}
}



/*删除调整*/
static void DeleteFixup(RBTree *T, Node *x) {
	while (x != (*T) && x->color == BLACK) {
		if (x == Parent(x)->left) {
			Node *w = Parent(x)->right;  //w 为x的兄弟结点
			if (w->color == RED) {          //case 1 兄弟结点为红色
				w->color = BLACK;
				x->parent->color = RED;
				LeftRotate(T, Parent(x));
				w = Parent(x)->right;
			}
			if (w == nil) break;
			if (Left(w)->color == BLACK && Right(w)->color == BLACK) {   //case2 兄弟结点的两个子结点都为黑
				w->color = RED;
				x = Parent(x);
			}
			else if (w->right->color == BLACK) {    //case3 w的左子树为红色,右子树为黑色
				w->color = RED;
				w->left->color = BLACK;
				RightRotate(T, w);
				w = Parent(x)->right;
			}
			w->color = x->parent->color;         //case 4 w的右子树为红色
			x->parent->color = BLACK;
			w->right->color = BLACK;
			LeftRotate(T, Parent(x));
		}
		else {  //对称 调整同上
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



/*查找某个结点*/
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

/*打印结点*/
void rbPrint(RBTree T) {
	if (T != NULL && T != nil) {
		rbPrint(T->left);
		printf("%d(%s)\n", T->key, (T->color == RED) ? "红" : "黑");
		rbPrint(T->right);
	}
}
/*查找最小最小结点*/
Node *rbMinKey(RBTree T) {
	Node *x = T;
	Node *p = nil;
	while (x != nil) {
		p = x;
		x = x->left;
	}
	return p == nil ? NULL : p;
}

/*查找最大结点*/
Node *rbMaxKey(RBTree T) {
	Node *x = T;
	Node *p = nil;
	while (x != nil) {
		p = x;
		x = x->right;
	}
	return p == nil ? NULL : p;
}

/*可视化显示*/
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

