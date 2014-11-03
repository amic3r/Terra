
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
	if(arr->len >= 1)
		TArrayResize(arr,TIntegerUpperPowerOfTwo(arr->len));
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
	memset(arr,0,sizeof(TArray));

	if(size) TArrayResize(arr,size);
}

void TArrayFree(TArray *arr,TFreeFunc func)
{
	if(arr) {
		TArrayEmpty(arr,func);

		free(arr->data);
		free(arr);
	}
}

int TArrayResize(TArray *arr, size_t _size)
{
	if(arr) {
		if(_size == 0) {
			free(arr->data);
			arr->size = arr->used = arr->len = 0;
			arr->data = 0;
		} else {
			if(!TRAlloc((void **) &arr->data,sizeof(char *) * _size)) return 1;

			if(_size > arr->size) memset(arr->data+arr->size,0,sizeof(char *) *(_size-arr->size));
			else arr->len = TMIN(arr->len,_size);

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
	cpy->len = arr->len;

	for(; i < arr->len; ++i) cpy->data[i] = data_cpy ? data_cpy(arr->data[i]) : arr->data[i];

	return cpy;
}

void TArrayCopyInplace(TArray *to, const TArray *arr, TCopyFunc data_cpy)
{
	size_t i = 0;
	if(!to || !arr) return;

	if(to->size < arr->size) TArrayResize(to,arr->len);

	to->used = arr->used;
	to->len = arr->len;

	for(; i < arr->len; ++i) to->data[i] = data_cpy ? data_cpy(arr->data[i]) : arr->data[i];
}

void TArrayEmpty(TArray *arr,TFreeFunc func)
{
	if(func) {
		size_t i = 0;
		for(; i < arr->len && arr->used; ++i) {
			if(arr->data[i]) {
				arr->used--;
				func(arr->data[i]);
				arr->data[i] = 0;
			}
		}
	}
	arr->len = 0;
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
	TArrayInsert(arr,data,arr->len);
	return arr->len-1;
}

void TArrayInsert(TArray *arr,void *data, size_t index)
{
	if(index >= arr->size) TArrayGrow(arr,index+1);
	if(!arr->data[index]) arr->used += 1;
	arr->data[index] = data;
	arr->len = TMAX(index+1,arr->len);
}

void TArrayMove(TArray *arr, size_t start, size_t end, int space)
{
	if(!arr || start > end || !TArrayValid(arr,start) || !space) return;

	if(end > arr->len) end = arr->len;

	if(space > 0) {
		int i;
		int li = (int)end + space - 1;

		if((size_t) li > arr->size) TArrayGrow(arr,li);

		for(i = end-1; i >= (int)start; i--) {
			arr->data[i+space] = arr->data[i];
			arr->data[i] = 0;
		}

		arr->len = (size_t)(li + 1);
	} else {
		size_t i;

		if((int)start + space < 0) start = (size_t)(-space);

		for(i = start; i < end; ++i) {
			arr->data[i+space] = arr->data[i];
			arr->data[i] = 0;
		}

		if(arr->len <= (arr->size >> 2)) TArrayShrink(arr);

		arr->len = TMAX(end,arr->len);
	}

}

void TArrayForeach(TArray *arr, TIterFunc func)
{
	size_t i = 0;
	for(; i < arr->len; ++i) func(arr->data[i]);
}

void *TArrayForeachData(TArray *arr, TDataIterFunc func,void *user_data)
{
	size_t i = 0;
	for(; i < arr->len; ++i) {
		void *value = func(arr->data[i],user_data);
		if(value) return value;
	}

	return 0;
}

size_t TArrayFind(TArray *arr, const void *data)
{
	size_t i = 0;

	for(; i < arr->len; ++i)
		if(data == arr->data[i])
			return i;

	return i;
}

void TArraySort(TArray *arr)
{
	//TODO
}

void *TArrayPopIndex(TArray *arr,size_t index) {
	void *data;

	if(index >= arr->len) return 0;

	data = arr->data[index];
	if(data) arr->used -= 1;

	for(; index < arr->len-1; ++index) arr->data[index] = arr->data[index+1];
	arr->len -= 1;

	if(arr->len <= (arr->size >> 2)) TArrayShrink(arr);

	return data;
}

void TArrayRemove(TArray *arr,size_t index)
{
	if(index >= arr->len) return;

	if(arr->data[index]) arr->used -= 1;

	memcpy(arr->data[index],arr->data[index+1],sizeof(void *) *(arr->len - (index + 1)));
	arr->len -= 1;

	if(arr->len <= (arr->size >> 2)) TArrayShrink(arr);
}

void TArrayRemoveFast(TArray *arr,size_t index)
{
	if(index >= arr->len) return;

	if(arr->data[index]) arr->used -= 1;

	arr->data[index] = arr->data[arr->len-1];
	arr->data[arr->len-1] = 0;
	arr->len -= 1;

	if(arr->len <= (arr->size >> 2)) TArrayShrink(arr);
}

void TArrayRemoveClear(TArray *arr,size_t index)
{
	if(index >= arr->len) return;

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
	if(arr->len >= 1)
		TIArrayResize(arr,TIntegerUpperPowerOfTwo(arr->len));
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
	arr->size = arr->len = 0;

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
			arr->len = arr->size = 0;
		} else {
			if(!TRAlloc((void **) &arr->data,sizeof(int) * _size)) return 1;

			if(_size > arr->size) memset(arr->data+arr->size,0,sizeof(int) *(_size-arr->size));
			else arr->len = TMIN(arr->len,_size);

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
	
	cpy->len = arr->len;

	for(; i < arr->len; ++i) cpy->data[i] = arr->data[i];

	return cpy;
}

void TIArrayCopyInplace(TIArray *to, const TIArray *arr)
{
	size_t i = 0;
	if(!to || !arr) return;

	if(to->size < arr->size) TIArrayGrow(to,arr->len);

	to->len = arr->len;

	for(; i < arr->len; ++i) to->data[i] = arr->data[i];
}

void TIArrayEmpty(TIArray *arr)
{
	arr->len = 0;
}

void TIArrayEmptyFull(TIArray *arr)
{
	arr->len = 0;
	free(arr->data);
	arr->data = 0;
	arr->size = 0;
}

size_t TIArrayAppend(TIArray *arr,int data)
{
	TIArrayInsert(arr,data,arr->len);
	return arr->len-1;
}

void TIArrayInsert(TIArray *arr, int data, size_t index)
{
	if(index >= arr->size) TIArrayGrow(arr,index+1);
	arr->data[index] = data;
	arr->len = TMAX(index+1,arr->len);
}

void TIArrayMove(TIArray *arr, size_t start, size_t end, int space)
{
	if(!arr || start > end || !TIArrayValid(arr,start) || !space) return;

	if(end > arr->len) end = arr->len;

	if(space > 0) {
		int i;
		int li = (int)end + space - 1;

		if((size_t) li > arr->size) TIArrayGrow(arr,li);

		for(i = end-1; i >= (int)start; i--) {
			arr->data[i+space] = arr->data[i];
			arr->data[i] = 0;
		}

		arr->len = (size_t)(li + 1);
	} else {
		size_t i;

		if((int)start + space < 0) start = (size_t)(-space);

		for(i = start; i < end; ++i) {
			arr->data[i+space] = arr->data[i];
			arr->data[i] = 0;
		}

		if(arr->len <= (arr->size >> 2)) TIArrayShrink(arr);

		arr->len = TMAX(end,arr->len);
	}

}

void TIArrayForeach(TArray *arr, TIterFunc func)
{
	size_t i = 0;
	for(; i < arr->len; ++i) func(&arr->data[i]);
}

void *TIArrayForeachData(TArray *arr, TDataIterFunc func,void *user_data)
{
	size_t i = 0;
	for(; i < arr->len; ++i) {
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

	if(index >= arr->len) return 0;

	data = arr->data[index];

	for(; index < arr->len-1; ++index) arr->data[index] = arr->data[index+1];
	arr->len -= 1;

	if(arr->len <= (arr->size >> 2)) TIArrayShrink(arr);

	return data;
}

void TIArrayRemove(TIArray *arr,size_t index)
{
	if(index >= arr->len) return;

	memcpy(arr->data + index,arr->data +(index+1),sizeof(int) *(arr->len - (index + 1)));
	arr->len -= 1;

	if(arr->len <= (arr->size >> 2)) TIArrayShrink(arr);
}

void TIArrayRemoveFast(TIArray *arr,size_t index)
{
	if(index >= arr->len) return;

	arr->data[index] = arr->data[arr->len-1];
	arr->data[arr->len-1] = 0;
	arr->len -= 1;

	if(arr->len <= (arr->size >> 2)) TIArrayShrink(arr);
}
