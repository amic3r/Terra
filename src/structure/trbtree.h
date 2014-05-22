
#ifndef __included_terra_rbtree_h
#define __included_terra_rbtree_h

// Red-Black Tree

typedef struct TRBTree TRBTree;

TRBTree *tRBTreeNew(TCompareFunc key_compare, TFreeFunc free_key, TFreeFunc free_data);
void tRBTreeFree(TRBTree *t);

void tRBTreeEmpty(TRBTree *t);

size_t tRBTreeSize(const TRBTree *t);

unsigned char tRBTreeInsert(TRBTree *t, void *key, void *data);
unsigned char tRBTreeReplace(TRBTree *t, void *key, void *data);
void tRBTreeErase(TRBTree *t, void *key);

void *tRBTreeFind(const TRBTree *t, void *key);
unsigned char tRBTreeExists(const TRBTree *t, void *key);

size_t tRBTreeMemUsage(const TRBTree *t);

void *tRBTreeDataTraverse(const TRBTree *t, TDataPairIterFunc f, void *udata);
void tRBTreeTraverse(const TRBTree *t, TPairIterFunc f);

// Red-Black Tree Iterator

typedef struct TRBTreeIterator TRBTreeIterator;

TRBTreeIterator *tRBTreeIteratorNew(const TRBTree *t);
void tRBTreeIteratorFree(TRBTreeIterator *iter);

int tRBTreeIteratorNext(TRBTreeIterator *iter, void **key, void **data);
int tRBTreeIteratorPrevious(TRBTreeIterator *iter, void **key, void **data);

#endif
