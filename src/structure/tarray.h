
#ifndef __included_terra_array_h
#define __included_terra_array_h

//------------- Ptr Array ---------------//

#include "tdefine.h"

typedef struct _TArray {
	void **data;
	size_t size,used;
	size_t lastindex;
} TArray;

TArray *TArrayNew(size_t size);
void TArrayInit(TArray *arr,size_t size);
void TArrayFree(TArray *arr,TFreeFunc func);

int TArrayResize(TArray *arr, size_t _size);

TArray *TArrayCopy(const TArray *arr, TCopyFunc data_cpy);
void TArrayCopyInplace(TArray *to, const TArray *arr, TCopyFunc data_cpy);

void TArrayEmpty(TArray *arr,TFreeFunc func);
void TArrayEmptyFull(TArray *arr,TFreeFunc func);

size_t TArrayAppend(TArray *arr,void *data);
void TArrayInsert(TArray *arr,void *data, size_t index);

void TArrayMove(TArray *arr, size_t start, size_t end, int space);

void TArrayForeach(TArray *arr, TIterFunc func);
void *TArrayForeachData(TArray *arr, TDataIterFunc func,void *user_data);

static inline unsigned char TArrayValid(TArray *arr,size_t index)
{
	return index < arr->lastindex;
}

static inline void *TArrayGet(TArray *arr,size_t index)
{
	if(!TArrayValid(arr,index)) return 0;
	return arr->data[index];
}

void TArraySort(TArray *arr);

void *TArrayPopIndex(TArray *arr,size_t index);

void TArrayRemove(TArray *arr,size_t index);
void TArrayRemoveFast(TArray *arr,size_t index); // grab end element and put it in place
void TArrayRemoveClear(TArray *arr,size_t index); // replace with 0

#define TArrayPush TArrayAppend
#define TArrayPop(a) TArrayPopIndex(a,a->lastindex);

//------------- Integer Array ---------------//

typedef struct _TIArray {
	int *data;
	size_t size;
	size_t lastindex;
} TIArray;

TIArray *TIArrayNew(size_t size);
void TIArrayInit(TIArray *arr,size_t size);
void TIArrayFree(TIArray *arr);

int TIArrayResize(TIArray *arr, size_t _size);

TIArray *TIArrayCopy(const TIArray *arr);
void TIArrayCopyInplace(TIArray *to, const TIArray *arr);

void TIArrayEmpty(TIArray *arr);
void TIArrayEmptyFull(TIArray *arr);

size_t TIArrayAppend(TIArray *arr,int data);
void TIArrayInsert(TIArray *arr,int data, size_t index);

void TIArrayMove(TIArray *arr, size_t start, size_t end, int space);

void TIArrayForeach(TArray *arr, TIterFunc func);
void *TIArrayForeachData(TArray *arr, TDataIterFunc func,void *user_data);

static inline unsigned char TIArrayValid(TIArray *arr,size_t index)
{
	return index < arr->lastindex;
}

static inline int TIntArrayGet(TIArray *arr,size_t index)
{
	if(!TIArrayValid(arr,index)) return 0;
	return arr->data[index];
}

void TIArraySort(TIArray *arr);

int TIArrayPopIndex(TIArray *arr,size_t index);
void TIArrayRemove(TIArray *arr,size_t index);
void TIArrayRemoveFast(TIArray *arr,size_t index); // grab end element and put it in place

#define TIArrayPush TIArrayAppend
#define TIArrayPop(a) TIArrayPopIndex(a,a->lastindex);

#endif
