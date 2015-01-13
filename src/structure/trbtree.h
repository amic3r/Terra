
#ifndef __included_terra_rbtree_h
#define __included_terra_rbtree_h

// Red-Black Tree

typedef struct TRBTree TRBTree;

TRBTree *TRBTreeNew(TCompareFunc key_compare, TFreeFunc free_key, TFreeFunc free_data);
void TRBTreeFree(TRBTree *t);

void TRBTreeEmpty(TRBTree *t);

size_t TRBTreeSize(const TRBTree *t);

unsigned char TRBTreeInsert(TRBTree *t, const void *key, const void *data);
const void *TRBTreeReplace(TRBTree *t, const void *key, const void *data);
void TRBTreeErase(TRBTree *t, const void *key);

const void *TRBTreeFind(const TRBTree *t, const void *key);
unsigned char TRBTreeExists(const TRBTree *t, const void *key);

size_t TRBTreeMemUsage(const TRBTree *t);

void *TRBTreeDataTraverse(const TRBTree *t, TDataPairIterFunc f, void *udata);
void TRBTreeTraverse(const TRBTree *t, TPairIterFunc f);

// Red-Black Tree Iterator

typedef struct TRBTreeIterator TRBTreeIterator;

TRBTreeIterator *TRBTreeIteratorNew(const TRBTree *t);
void TRBTreeIteratorFree(TRBTreeIterator *iter);

int TRBTreeIteratorNext(TRBTreeIterator *iter, const void **key, const void **data);
int TRBTreeIteratorPrevious(TRBTreeIterator *iter, const void **key, const void **data);

#endif
