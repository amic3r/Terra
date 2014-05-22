
#include "stdafx.h"

#include "trbtree.h"

#include "talloc.h"

enum color_t {Red,Black};

typedef struct TRBTreeNode
{
	void *key;
	void *data;
	int color;

	struct TRBTreeNode *left, *right, *parent;
} TRBTreeNode;

TRBTreeNode *tRBTreeNodeNew(void)
{
	TRBTreeNode *n = (TRBTreeNode *) TAlloc(sizeof(TRBTreeNode));

	if(n) {
		n->left = n->right = n->parent = 0;
		n->color = Red;
		n->key = n->data = 0;
	}

	return n;
}

void tRBTreeNodeFree(TRBTreeNode *n, TFreeFunc fk, TFreeFunc fd)
{
	if(n) {
		if(fk) fk(n->key);
		if(fd) fd(n->data);
		tRBTreeNodeFree(n->left, fk, fd);
		tRBTreeNodeFree(n->right, fk, fd);
		free(n);
	}
}

size_t tRBTreeNodeMemUsage(const TRBTreeNode *n)
{
	if(n) return sizeof(*n) + sizeof(*n->left) + sizeof(*n->right);
	
	return 0;
}

struct TRBTree {
	TRBTreeNode *root;

	size_t size;

	TCompareFunc kcomp;

	TFreeFunc fk;
	TFreeFunc fd;
};

int ptr_comp(const void *d1,const void *d2)
{
	if(d1 > d2) return 1;
	else if(d1 < d2) return -1;
	return 0;
}

TRBTree *tRBTreeNew(TCompareFunc key_compare, TFreeFunc free_key, TFreeFunc free_data)
{
	TRBTree *t = (TRBTree *) TAlloc(sizeof(TRBTree));
	
	if(t) {
		t->root = 0;
		t->size = 0;
		t->kcomp = key_compare ? key_compare : ptr_comp;
		t->fk = free_key;
		t->fd = free_data;
	}

	return t;
}

void tRBTreeFree(TRBTree *t)
{
	if(t) {
		tRBTreeNodeFree(t->root,t->fk,t->fd);
		free(t);
	}
}

void tRBTreeEmpty(TRBTree *t)
{
	tRBTreeNodeFree(t->root,t->fk,t->fd);
	t->root = 0;
	t->size = 0;
}

size_t tRBTreeSize(const TRBTree *t)
{
	return t->size;
}

inline const TRBTreeNode *tRBTreeLeftMost(const TRBTree *t, const TRBTreeNode *c)
{
	if(!c) {
		if(!t->root) return 0;
		c = t->root;
	}

	while(c->left) c = c->left;

	return c;
}

inline const TRBTreeNode *tRBTreeRightMost(const TRBTree *t, const TRBTreeNode *c)
{
	if(!c) {
		if(!t->root) return 0;
		c = t->root;
	}

	while(c->right) c = c->right;

	return c;
}

void tRBTreeRotateLeft(TRBTree *t, TRBTreeNode *x)
{
	TRBTreeNode *y = x->right;

	/* establish x->right link */
	x->right = y->left;
	if (y->left)
		y->left->parent = x;

	/* establish y->parent link */
	y->parent = x->parent;

	if (x->parent) {
		if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
	} else {
		t->root = y;
	}

	/* link x and y */
	y->left = x;
	x->parent = y;
}

void tRBTreeRotateRight(TRBTree *t, TRBTreeNode *x)
{
	TRBTreeNode *y = x->left;

	/* establish x->left link */
	x->left = y->right;
	if (y->right)
		y->right->parent = x;

	/* establish y->parent link */
	y->parent = x->parent;

	if (x->parent) {
		if (x == x->parent->right)
			x->parent->right = y;
		else
			x->parent->left = y;
	} else {
		t->root = y;
	}

	/* link x and y */
	y->right = x;
	x->parent = y;
}

void tRBTreeInsertFixup(TRBTree *t, TRBTreeNode *x)
{
	/* check Red-Black properties */
	while (x != t->root && x->parent->color == Red) {
		/* we have a violation */
		if (x->parent == x->parent->parent->left) {
			TRBTreeNode *y = x->parent->parent->right;

			if (y && y->color == Red) {
				/* uncle is RED */
				y->color = x->parent->color = Black;
				x->parent->parent->color = Red;
				x = x->parent->parent;
			} else {
				/* uncle is BLACK */
				if (x == x->parent->right) {
					/* make x a left child */
					x = x->parent;
					tRBTreeRotateLeft(t, x);
				}

				/* recolor and rotate */
				x->parent->color = Black;
				x->parent->parent->color = Red;
				tRBTreeRotateRight(t, x->parent->parent);
			}
		} else {
			/* mirror image of above code */
			TRBTreeNode *y = x->parent->parent->left;

			if (y && y->color == Red) {
				/* uncle is RED */
				y->color = x->parent->color = Black;
				x->parent->parent->color = Red;
				x = x->parent->parent;
			} else {
				/* uncle is BLACK */
				if (x == x->parent->left) {
					x = x->parent;
					tRBTreeRotateRight(t, x);
				}

				x->parent->color = Black;
				x->parent->parent->color = Red;
				tRBTreeRotateLeft(t, x->parent->parent);
			}
		}
	}
	t->root->color = Black;
}

inline TRBTreeNode *tRBTreeFindNode(const TRBTree *t, void *key)
{
	TRBTreeNode *current = t->root;

	while (current) {
		int cmp = t->kcomp(key, current->key);
		if (cmp < 0)
			current = current->left;
		else if (cmp > 0)
			current = current->right;
		else
			return current;
	}

	return 0;
}

unsigned char tRBTreeReplace(TRBTree *t, void *key, void *data)
{
	TRBTreeNode *current = tRBTreeFindNode(t, key);
	if (!current) return 1;

	current->data = data;
	return 0;
}

unsigned char tRBTreeInsert(TRBTree *t, void *key, void *data)
{
	TRBTreeNode *current = t->root, *parent = 0, *x = 0;
	int ret;

	x = tRBTreeNodeNew();
	if(!x) return 1;

	/* find future parent */
	while (current) {
		parent = current;

		ret = t->kcomp(key, current->key);
		if (ret < 0)
			current = current->left;
		else if (ret > 0)
			current = current->right;
		else
			return 1;
	}

	/* setup new node */
	x->parent = parent;
	x->key = key;
	x->data = data;

	/* insert node in tree */
	if (parent) {
		if (ret <= 0)
			parent->left = x;
		else
			parent->right = x;
	} else {
		t->root = x;
	}

	t->size++;

	tRBTreeInsertFixup(t, x);

	return 0;
}

void tRBTreeDeleteFixup(TRBTree *t, TRBTreeNode *x)
{
	if (!x) return;

	while (x != t->root && x->color == Black) {
		if (x == x->parent->left) {
			TRBTreeNode *w = x->parent->right;

			if (w->color == Red) {
				w->color = Black;
				x->parent->color = Red;
				tRBTreeRotateLeft(t, x->parent);
				w = x->parent->right;
			}

			if (w->left->color == Black && w->right->color == Black) {
				w->color = Red;
				x = x->parent;
			} else {
				if (w->right->color == Black) {
					w->left->color = Black;
					w->color = Red;
					tRBTreeRotateRight(t, w);
					w = x->parent->right;
				}

				w->color = x->parent->color;
				x->parent->color = w->right->color = Black;
				tRBTreeRotateLeft(t, x->parent);
				x = t->root;
			}
		} else {
			TRBTreeNode *w = x->parent->left;

			if (w->color == Red) {
				w->color = Black;
				x->parent->color = Red;
				tRBTreeRotateRight(t, x->parent);
				w = x->parent->left;
			}

			if (w->right->color == Black && w->left->color == Black) {
				w->color = Red;
				x = x->parent;
			} else {
				if (w->left->color == Black) {
					w->right->color = Black;
					w->color = Red;
					tRBTreeRotateLeft(t, w);
					w = x->parent->left;
				}

				w->color = x->parent->color;
				x->parent->color = w->left->color = Black;
				tRBTreeRotateRight(t, x->parent);
				x = t->root;
			}
		}
	}
	x->color = Black;
}

void tRBTreeKillNode(TRBTree *t, TRBTreeNode *z)
{
	TRBTreeNode *x, *y;

	if (!z->left || !z->right) {
		/* y has a null node as a child */
		y = z;
	} else {
		/* find tree successor with a null node as a child */
		y = z->right;

		while (y->left)
			y = y->left;
	}

	/* x is y's only child */
	if (y->left)
		x = y->left;
	else
		x = y->right;

	/* remove y from the parent chain */
	if (x) x->parent = y->parent;

	if (y->parent) {
		if (y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
	} else
		t->root = x;

	if (y != z) {
		t->fk(z->key);
		z->key = y->key;
		z->data = y->data;
	} else {
		t->fk(y->key);
	}

	if (y->color == Black)
		tRBTreeDeleteFixup(t, x);

	t->size--;

	y->left = y->right = 0;
	free(y);
}

void tRBTreeErase(TRBTree *t, void *key)
{
	TRBTreeNode *z =  tRBTreeFindNode(t,key);

	/*  delete node z from tree */

	/* find node in tree */
	if(z) tRBTreeKillNode(t, z);
}

void *tRBTreeFind(const TRBTree *t, void *key)
{
	TRBTreeNode *current = tRBTreeFindNode(t,key);

	return current ? current->data : 0;
}

unsigned char tRBTreeExists(const TRBTree *t, void *key)
{
	TRBTreeNode *current = tRBTreeFindNode(t, key);
	
	return current != 0;
}

size_t tRBTreeMemUsage(const TRBTree *t)
{
	size_t ret = sizeof(*t);
	if (t->root) ret += tRBTreeNodeMemUsage(t->root);

	return ret;
}

void *tRBTreeDataTraverse(const TRBTree *t, TDataPairIterFunc f, void *udata)
{
	TRBTreeIterator *iter;
	void *key, *data;
	void *d = 0;
	
	if(!f) return 0;

	iter = tRBTreeIteratorNew(t);

	while(!d && tRBTreeIteratorNext(iter,&key,&data)) d = f(key,data,udata);

	tRBTreeIteratorFree(iter);

	return d;
}

void tRBTreeTraverse(const TRBTree *t, TPairIterFunc f)
{
	TRBTreeIterator *iter;
	void *key, *data;
	
	if(!f) return;

	iter = tRBTreeIteratorNew(t);

	while(tRBTreeIteratorNext(iter,&key,&data)) f(key,data);

	tRBTreeIteratorFree(iter);
}

struct TRBTreeIterator {
	const TRBTree *t;
	const TRBTreeNode *c;
	unsigned char beginning, end;
};

TRBTreeIterator *tRBTreeIteratorNew(const TRBTree *t)
{
	TRBTreeIterator *iter = (TRBTreeIterator *) TAlloc(sizeof(TRBTreeIterator));

	iter->t = t;
	iter->c = 0;
	iter->beginning = iter->end = 0;

	return iter;
}

void tRBTreeIteratorFree(TRBTreeIterator *iter)
{
	free(iter);
}

int tRBTreeIteratorNext(TRBTreeIterator *iter, void **key, void **data)
{
	iter->beginning = 0;
	if(iter->end) return 0;

	if(!iter->c) {
		iter->c = tRBTreeLeftMost(iter->t, 0);
		iter->beginning = 1;
	} else if(iter->c->right) {
		iter->c = iter->c->right;
		iter->c = tRBTreeLeftMost(iter->t,iter->c);
	} else {
		unsigned char didbreak = 0;
		const TRBTreeNode *c = iter->c;

		while (iter->c->parent) {
			const TRBTreeNode *l = iter->c;
			iter->c = iter->c->parent;
			if(iter->c->left == l) {
				didbreak = 1;
				break;
			}
		}

		if(!didbreak) {
			iter->end = 1;
			iter->c = c;
			return 0;
		}
	}

	if(key) *key = iter->c->key;
	if(data) *data = iter->c->data;

	return 1;
}

int tRBTreeIteratorPrevious(TRBTreeIterator *iter, void **key, void **data)
{
	iter->end = 0;
	if(iter->beginning) {
		return 0;
	}

	if(!iter->c) {
		iter->c = tRBTreeRightMost(iter->t, 0);
		iter->end = 1;
	} else if(iter->c->left) {
		iter->c = iter->c->left;
		iter->c = tRBTreeRightMost(iter->t,iter->c);
	} else {
		unsigned char didbreak = 0;
		const TRBTreeNode *c = iter->c;

		while (iter->c->parent) {
			const TRBTreeNode *l = iter->c;
			iter->c = iter->c->parent;
			if(iter->c->right == l) {
				didbreak = 1;
				break;
			}
		}

		if(!didbreak) {
			iter->beginning = 1;
			iter->c = c;
			return 0;
		}
	}

	if(key) *key = iter->c->key;
	if(data) *data = iter->c->data;

	return 1;
}
