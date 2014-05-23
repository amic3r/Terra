
#ifndef __included_terra_rbtree_h
#define __included_terra_rbtree_h

// Red-Black Tree

typedef struct TRBTree TRBTree;

TRBTree *TRBTreeNew(TCompareFunc key_compare, TFreeFunc free_key, TFreeFunc free_data);
void TRBTreeFree(TRBTree *t);

void TRBTreeEmpty(TRBTree *t);

size_t TRBTreeSize(const TRBTree *t);

unsigned char TRBTreeInsert(TRBTree *t, void *key, void *data);
unsigned char TRBTreeReplace(TRBTree *t, void *key, void *data);
void TRBTreeErase(TRBTree *t, void *key);

void *TRBTreeFind(const TRBTree *t, void *key);
unsigned char TRBTreeExists(const TRBTree *t, void *key);

size_t TRBTreeMemUsage(const TRBTree *t);

void *TRBTreeDataTraverse(const TRBTree *t, TDataPairIterFunc f, void *udata);
void TRBTreeTraverse(const TRBTree *t, TPairIterFunc f);

// Red-Black Tree Iterator

typedef struct TRBTreeIterator TRBTreeIterator;

TRBTreeIterator *TRBTreeIteratorNew(const TRBTree *t);
void TRBTreeIteratorFree(TRBTreeIterator *iter);

int TRBTreeIteratorNext(TRBTreeIterator *iter, void **key, void **data);
int TRBTreeIteratorPrevious(TRBTreeIterator *iter, void **key, void **data);

#endif
