#include "stdafx.h"

#include "rbtree_test.h"

#include "structure/trbtree.h"
#include "utility/tinteger.h"

#include "test_utils.h"

void rbtree_test_iteration(const TRBTree *tree)
{
	TRBTreeIterator *iter;
	int *key, *data;
	int i = 0;

	// init
	iter = tRBTreeIteratorNew(tree);
	if(!testNotNull(iter,"Initializing iterator\t\t")) return;

	// next
	while(tRBTreeIteratorNext(iter,(void **) &key,(void **) &data))
	{
		TAssert(*key == *data && *key == i++);
	}

	// previous
	--i;
	while(tRBTreeIteratorPrevious(iter,(void **) &key,(void **) &data))
	{
		TAssert(*key == *data && *key == --i);
	}

	// free
	tRBTreeIteratorFree(iter);
}

void rbtree_test_all(void)
{
	TRBTree *tree;
	int i;
	unsigned char f = 0;
	int testv = 50;

	// init
	tree = tRBTreeNew((TCompareFunc) TIntegerCompare,free,free);
	if(!testNotNull(tree,"Initializing tree\t\t")) return;

	// insert
	for(i = 0;!f && i < 100; ++i) f = tRBTreeInsert(tree,TIntegerToPtr(i),TIntegerToPtr(i));
	testReport(!f,"Testing Insertion\t\t");

	// size
	testReport(tRBTreeSize(tree) == 100,"Testing Size\t\t");

	// find
	testReport(*(int *)tRBTreeFind(tree,&testv) == 50,"Testing Find function\t\t");

	// exists
	testv = 34;
	testReport(tRBTreeExists(tree,&testv),"Testing Exists function\t\t");

	// mem
	// TODO

	// data traversal
	// TODO

	// traversal
	// TODO

	// iteration
	rbtree_test_iteration(tree);

	// replace
	// TODO

	// removal
	for(i = 0; i < 94; ++i) tRBTreeErase(tree,&i);
	testReport(tRBTreeSize(tree) == 6,"Testing Erase Function\t\t");;

	// empty
	tRBTreeEmpty(tree);
	testReport(tRBTreeSize(tree) == 0,"Ensuring that the tree is empty\t\t");;

	// free
	tRBTreeFree(tree);
}

void rbtree_test(void)
{
	TLogReport(T_LOG_PROGRESS,0,"Testing Red Black Tree...\n");

	rbtree_test_all();

	TLogReport(T_LOG_PROGRESS,0,"Red Black Tree tests completed.\n");
}
