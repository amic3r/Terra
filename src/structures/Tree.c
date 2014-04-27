
#include "stdafx.h"

#include "tree.h"

//Red-Black Tree

enum color_t {Red,Black};

typedef struct _TreeNode
{
	void *key;
	void *data;
	int color;

	struct _TreeNode *left, *right, *parent;
} TreeNode;

struct _Tree {
	TreeNode *root;

	size_t size;

	unsigned char (*comp) (const void *,const void*);
};

unsigned char ptr_comp(const void *d1,const void *d2)
{
	return d1 < d2;
}

Tree *tree_new(unsigned char (*comparison) (const void *,const void*))
{
	Tree *t = (Tree *) malloc(sizeof(Tree));
	AppAssert(t);

	t->root = 0;
	t->size = 0;
	t->comp = comparison ? comparison : ptr_comp;

	return t;
}

Tree *tree_free(Tree *t, void (*free_key)(void *, void *))
{
	
}
