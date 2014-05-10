
#include "stdafx.h"

#include "tarray.h"

#include "utility/tinteger.h"

#include "talloc.h"

static inline void TArrayGrow(TArray *arr,size_t minsize)
{
	TArrayResize(arr,TIntegerUpperPowerOfTwo(minsize));
}

static inline void TArrayShrink(TArray *arr)
{
	if(arr->lastindex >= 1)
		TArrayResize(arr,TIntegerUpperPowerOfTwo(arr->lastindex));
}

TArray *TArrayNew(size_t size)
{
	TArray *arr = (TArray *) TAlloc(sizeof(TArray));
	if(!arr) return 0;

	TArrayInit(arr,size);

	return arr;
}

void TArrayInit(TArray *arr,size_t size)
{
	arr->size = size;
	arr->used = arr->lastindex = 0;
	arr->data = 0;

	if(size) TArrayResize(arr,size);
}

void TArrayFree(TArray *arr,TFreeFunc func)
{
	if(arr) {
		if(func) {
			TArrayEmpty(arr,func);
		}

		free(arr->data);
		free(arr);
	}
}

int TArrayResize(TArray *arr, size_t _size)
{
	if(arr) {
		if(_size == 0) {
			free(arr->data);
			arr->size = arr->used = arr->lastindex = 0;
			arr->data = 0;
		} else {
			if(!TRAlloc((void **) &arr->data,sizeof(void *) * _size)) return 1;

			if(_size > arr->size) memset(arr->data+arr->size,0,sizeof(void *) *(_size-arr->size));
			else arr->lastindex = min(arr->lastindex,_size);

			arr->size = _size;
		}
	}

	return 0;
}

TArray *TArrayCopy(const TArray *arr, TCopyFunc data_cpy)
{
	TArray *cpy;
	size_t i = 0;
	
	if(!arr) return 0;

	cpy = TArrayNew(arr->size);
	
	cpy->used = arr->used;
	cpy->lastindex = arr->lastindex;

	for(; i < arr->lastindex; ++i) cpy->data[i] = data_cpy ? data_cpy(arr->data[i]) : arr->data[i];

	return cpy;
}

void TArrayCopyInplace(TArray *to, const TArray *arr, TCopyFunc data_cpy)
{
	size_t i = 0;
	if(!to || !arr) return;

	if(to->size < arr->size) TArrayResize(to,arr->lastindex);

	to->used = arr->used;
	to->lastindex = arr->lastindex;

	for(; i < arr->lastindex; ++i) to->data[i] = data_cpy ? data_cpy(arr->data[i]) : arr->data[i];
}

void TArrayEmpty(TArray *arr,TFreeFunc func)
{
	size_t i = 0;
	for(; i < arr->lastindex && arr->used; ++i) {
		if(arr->data[i]) {
			arr->used--;
			func(arr->data[i]);
			arr->data[i] = 0;
		}
	}
	arr->lastindex = 0;
}

void TArrayEmptyFull(TArray *arr,TFreeFunc func)
{
	TArrayEmpty(arr,func);
	free(arr->data);
	arr->data = 0;
	arr->size = 0;
}

size_t TArrayAppend(TArray *arr,void *data)
{
	TArrayInsert(arr,data,arr->lastindex);
	return arr->lastindex-1;
}

void TArrayInsert(TArray *arr,void *data, size_t index)
{
	if(index >= arr->size) TArrayGrow(arr,index+1);
	if(!arr->data[index]) arr->used += 1;
	arr->data[index] = data;
	arr->lastindex = max(index+1,arr->lastindex);
}

void TArrayMove(TArray *arr, size_t start, size_t end, int space)
{
	if(!arr || start > end || !TArrayValid(arr,start) || !space) return;

	if(end > arr->lastindex) end = arr->lastindex;

	if(space > 0) {
		int i;
		int li = (int)end + space - 1;

		if((size_t) li > arr->size) TArrayGrow(arr,li);

		for(i = end-1; i >= (int)start; i--) {
			arr->data[i+space] = arr->data[i];
			arr->data[i] = 0;
		}

		arr->lastindex = (size_t)(li + 1);
	} else {
		size_t i;

		if((int)start + space < 0) start = (size_t)(-space);

		for(i = start; i < end; ++i) {
			arr->data[i+space] = arr->data[i];
			arr->data[i] = 0;
		}

		if(arr->lastindex <= (arr->size >> 2)) TArrayShrink(arr);

		arr->lastindex = max(end,arr->lastindex);
	}

}

void TArrayForeach(TArray *arr, TIterFunc func)
{
	size_t i = 0;
	for(; i < arr->lastindex; ++i) func(arr->data[i]);
}

void *TArrayForeachData(TArray *arr, TDataIterFunc func,void *user_data)
{
	size_t i = 0;
	for(; i < arr->lastindex; ++i) {
		void *value = func(arr->data[i],user_data);
		if(value) return value;
	}

	return 0;
}

void TArraySort(TArray *arr)
{
	//TODO
}

void *TArrayPopIndex(TArray *arr,size_t index) {
	void *data;

	if(index >= arr->lastindex) return 0;

	data = arr->data[index];
	if(data) arr->used -= 1;

	for(; index < arr->lastindex-1; ++index) arr->data[index] = arr->data[index+1];
	arr->lastindex -= 1;

	if(arr->lastindex <= (arr->size >> 2)) TArrayShrink(arr);

	return data;
}

void TArrayRemove(TArray *arr,size_t index)
{
	if(index >= arr->lastindex) return;

	if(arr->data[index]) arr->used -= 1;

	for(; index < arr->lastindex-1; ++index) arr->data[index] = arr->data[index+1];
	arr->lastindex -= 1;

	if(arr->lastindex <= (arr->size >> 2)) TArrayShrink(arr);
}

void TArrayRemoveFast(TArray *arr,size_t index)
{
	if(index >= arr->lastindex) return;

	if(arr->data[index]) arr->used -= 1;

	arr->data[index] = arr->data[arr->lastindex-1];
	arr->data[arr->lastindex-1] = 0;
	arr->lastindex -= 1;

	if(arr->lastindex <= (arr->size >> 2)) TArrayShrink(arr);
}

void TArrayRemoveClear(TArray *arr,size_t index)
{
	if(index >= arr->lastindex) return;

	if(arr->data[index]) arr->used -= 1;

	arr->data[index] = 0;
}

//------------- Integer TArray ---------------//

static inline void TIArrayGrow(TIArray *arr,size_t minsize)
{
	TIArrayResize(arr,TIntegerUpperPowerOfTwo(minsize));
}

static inline void TIArrayShrink(TIArray *arr)
{
	if(arr->lastindex >= 1)
		TIArrayResize(arr,TIntegerUpperPowerOfTwo(arr->lastindex));
}

TIArray *TIArrayNew(size_t size)
{
	TIArray *arr = (TIArray *) TAlloc(sizeof(TIArray));
	if(!arr) return 0;

	TIArrayInit(arr,size);

	return arr;
}

void TIArrayInit(TIArray *arr,size_t size)
{
	arr->data = 0;
	arr->size = arr->lastindex = 0;

	TIArrayResize(arr,size);
}

void TIArrayFree(TIArray *arr)
{
	if(arr) {
		free(arr->data);
		free(arr);
	}
}

int TIArrayResize(TIArray *arr, size_t _size)
{
	if(arr) {
		if(_size == 0) {
			free(arr->data);
			arr->data = 0;
			arr->lastindex = arr->size = 0;
		} else {
			if(!TRAlloc((void **) &arr->data,sizeof(int) * _size)) return 1;

			if(_size > arr->size) memset(arr->data+arr->size,0,sizeof(int) *(_size-arr->size));
			else arr->lastindex = min(arr->lastindex,_size);

			arr->size = _size;
		}
	}

	return 0;
}

TIArray *TIArrayCopy(const TIArray *arr)
{
	TIArray *cpy;
	size_t i = 0;
	
	if(!arr) return 0;

	cpy = TIArrayNew(arr->size);
	
	cpy->lastindex = arr->lastindex;

	for(; i < arr->lastindex; ++i) cpy->data[i] = arr->data[i];

	return cpy;
}

void TIArrayCopyInplace(TIArray *to, const TIArray *arr)
{
	size_t i = 0;
	if(!to || !arr) return;

	if(to->size < arr->size) TIArrayGrow(to,arr->lastindex);

	to->lastindex = arr->lastindex;

	for(; i < arr->lastindex; ++i) to->data[i] = arr->data[i];
}

void TIArrayEmpty(TIArray *arr)
{
	arr->lastindex = 0;
}

void TIArrayEmptyFull(TIArray *arr)
{
	arr->lastindex = 0;
	free(arr->data);
	arr->data = 0;
	arr->size = 0;
}

size_t TIArrayAppend(TIArray *arr,int data)
{
	TIArrayInsert(arr,data,arr->lastindex);
	return arr->lastindex-1;
}

void TIArrayInsert(TIArray *arr, int data, size_t index)
{
	if(index >= arr->size) TIArrayGrow(arr,index+1);
	arr->data[index] = data;
	arr->lastindex = max(index+1,arr->lastindex);
}

void TIArrayMove(TIArray *arr, size_t start, size_t end, int space)
{
	if(!arr || start > end || !TIArrayValid(arr,start) || !space) return;

	if(end > arr->lastindex) end = arr->lastindex;

	if(space > 0) {
		int i;
		int li = (int)end + space - 1;

		if((size_t) li > arr->size) TIArrayGrow(arr,li);

		for(i = end-1; i >= (int)start; i--) {
			arr->data[i+space] = arr->data[i];
			arr->data[i] = 0;
		}

		arr->lastindex = (size_t)(li + 1);
	} else {
		size_t i;

		if((int)start + space < 0) start = (size_t)(-space);

		for(i = start; i < end; ++i) {
			arr->data[i+space] = arr->data[i];
			arr->data[i] = 0;
		}

		if(arr->lastindex <= (arr->size >> 2)) TIArrayShrink(arr);

		arr->lastindex = max(end,arr->lastindex);
	}

}

void TIArrayForeach(TArray *arr, TIterFunc func)
{
	size_t i = 0;
	for(; i < arr->lastindex; ++i) func(&arr->data[i]);
}

void *TIArrayForeachData(TArray *arr, TDataIterFunc func,void *user_data)
{
	size_t i = 0;
	for(; i < arr->lastindex; ++i) {
		void *value = func(&arr->data[i],user_data);
		if(value) return value;
	}

	return 0;
}

void TIArraySort(TIArray *arr)
{
	//TODO
}

int TIArrayPopIndex(TIArray *arr,size_t index)
{
	int data;

	if(index >= arr->lastindex) return 0;

	data = arr->data[index];

	for(; index < arr->lastindex-1; ++index) arr->data[index] = arr->data[index+1];
	arr->lastindex -= 1;

	if(arr->lastindex <= (arr->size >> 2)) TIArrayShrink(arr);

	return data;
}

void TIArrayRemove(TIArray *arr,size_t index)
{
	if(index >= arr->lastindex) return;

	for(; index < arr->lastindex-1; ++index) arr->data[index] = arr->data[index+1];
	arr->lastindex -= 1;

	if(arr->lastindex <= (arr->size >> 2)) TIArrayShrink(arr);
}

void TIArrayRemoveFast(TIArray *arr,size_t index)
{
	if(index >= arr->lastindex) return;

	arr->data[index] = arr->data[arr->lastindex-1];
	arr->data[arr->lastindex-1] = 0;
	arr->lastindex -= 1;

	if(arr->lastindex <= (arr->size >> 2)) TIArrayShrink(arr);
}
