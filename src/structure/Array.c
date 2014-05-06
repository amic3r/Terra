
#include "stdafx.h"

#include "Array.h"

#include "utility/integer.h"

void array_grow(Array *arr,size_t minsize)
{
	if(!arr->size) {
		arr->data = (void **) malloc(sizeof(void *) * minsize);
		AppAssert(arr->data);

		arr->size = minsize;
		memset(arr->data,0,sizeof(void *) * minsize);
	} else {
		size_t oldsize = arr->size;
		arr->size = upper_power_of_two(minsize);

		arr->data = (void **) realloc(arr->data,sizeof(void *) * arr->size);
		AppAssert(arr->data);

		memset(arr->data+oldsize,0,sizeof(void *) *(arr->size-oldsize));
	}
}

void array_shrink(Array *arr)
{
	if(arr->lastindex >= 1) {
		arr->size = upper_power_of_two(arr->lastindex);

		arr->data = (void **) realloc(arr->data,sizeof(void *) * arr->size);
		AppAssert(arr->data);
	}
}

Array *array_new(size_t size)
{
	Array *arr = (Array *) malloc(sizeof(Array));
	AppAssert(arr);

	array_init(arr,size);

	return arr;
}

Array *array_copy(const Array *arr, void *(*data_cpy)(void *))
{
	Array *cpy;
	size_t i = 0;
	
	if(!arr) return 0;

	cpy = array_new(arr->size);
	
	cpy->used = arr->used;
	cpy->lastindex = arr->lastindex;

	for(; i < arr->lastindex; ++i) cpy->data[i] = data_cpy ? data_cpy(arr->data[i]) : arr->data[i];

	return cpy;
}

void array_copy_inplace(Array *to, const Array *arr, void *(*data_cpy)(void *))
{
	size_t i = 0;
	if(!to || !arr) return;

	if(to->size < arr->size) array_grow(to,arr->lastindex);

	to->used = arr->used;
	to->lastindex = arr->lastindex;

	for(; i < arr->lastindex; ++i) to->data[i] = data_cpy ? data_cpy(arr->data[i]) : arr->data[i];
}

void array_init(Array *arr,size_t size)
{
	if(size) {
		arr->data = (void **) malloc(sizeof(void *) * size);
		AppAssert(arr->data);

		memset(arr->data,0,size);
	} else {
		arr->data = 0;
	}

	arr->size = size;
	arr->used = 0;
	arr->lastindex = 0;
}

void array_free(Array *arr,TFreeFunc func)
{
	if(arr) {
		if(func) {
			size_t i = 0;
			for(; i < arr->size && arr->used; ++i) {
				if(arr->data[i]) {
					arr->used--;
					func(arr->data[i]);
					arr->data[i] = 0;
				}
			}
		}

		free(arr->data);
		free(arr);
	}
}

void array_empty(Array *arr,TFreeFunc func)
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

void array_empty_full(Array *arr,TFreeFunc func)
{
	array_empty(arr,func);
	free(arr->data);
	arr->data = 0;
	arr->size = 0;
}

size_t array_append(Array *arr,void *data)
{
	array_insert(arr,data,arr->lastindex);
	return arr->lastindex-1;
}

void array_insert(Array *arr,void *data, size_t index)
{
	if(index >= arr->size) array_grow(arr,index+1);
	if(!arr->data[index]) arr->used += 1;
	arr->data[index] = data;
	arr->lastindex = max(index+1,arr->lastindex);
}

void array_move(Array *arr, size_t start, size_t end, int space)
{
	if(!arr || start > end || !array_valid(arr,start) || !space) return;

	if(end > arr->lastindex) end = arr->lastindex;

	if(space > 0) {
		int i;
		int li = (int)end + space - 1;

		if(li > arr->size) array_grow(arr,li);

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

		if(arr->lastindex <= (arr->size >> 2)) array_shrink(arr);

		arr->lastindex = max(end,arr->lastindex);
	}

}

void *array_foreach(Array *arr, void *(*func)(void *,void *),void *user_data)
{
	size_t i = 0;
	for(; i < arr->lastindex; ++i) {
		void *value = func(arr->data[i],user_data);
		if(value) return value;
	}

	return 0;
}

void *array_pop(Array *arr,size_t index) {
	void *data;

	if(index >= arr->lastindex) return 0;

	data = arr->data[index];
	if(data) arr->used -= 1;

	for(; index < arr->lastindex-1; ++index) arr->data[index] = arr->data[index+1];
	arr->lastindex -= 1;

	if(arr->lastindex <= (arr->size >> 2)) array_shrink(arr);

	return data;
}

void array_sort(Array *arr)
{
	//TODO
}

void array_remove(Array *arr,size_t index)
{
	if(index >= arr->lastindex) return;

	if(arr->data[index]) arr->used -= 1;

	for(; index < arr->lastindex-1; ++index) arr->data[index] = arr->data[index+1];
	arr->lastindex -= 1;

	if(arr->lastindex <= (arr->size >> 2)) array_shrink(arr);
}

void array_remove_fast(Array *arr,size_t index)
{
	if(index >= arr->lastindex) return;

	if(arr->data[index]) arr->used -= 1;

	arr->data[index] = arr->data[arr->lastindex-1];
	arr->data[arr->lastindex-1] = 0;
	arr->lastindex -= 1;

	if(arr->lastindex <= (arr->size >> 2)) array_shrink(arr);
}

void array_remove_clear(Array *arr,size_t index)
{
	if(index >= arr->lastindex) return;

	if(arr->data[index]) arr->used -= 1;

	arr->data[index] = 0;
}

//------------- Integer Array ---------------//

void int_array_grow(IntArray *arr,size_t minsize)
{
	if(!arr->size) {
		arr->data = (int *) malloc(sizeof(int) * minsize);
		AppAssert(arr->data);

		arr->size = minsize;
		memset(arr->data,0,sizeof(int) * minsize);
	} else {
		size_t oldsize = arr->size;
		arr->size = upper_power_of_two(minsize);

		arr->data = (int *) realloc(arr->data,sizeof(int) * arr->size);
		AppAssert(arr->data);

		memset(arr->data+oldsize,0,sizeof(int) *(arr->size-oldsize));
	}
}

void int_array_shrink(IntArray *arr)
{
	if(arr->lastindex >= 1) {
		arr->size = upper_power_of_two(arr->lastindex);

		arr->data = (int *) realloc(arr->data,sizeof(int) * arr->size);
		AppAssert(arr->data);
	}
}

IntArray *int_array_new(size_t size)
{
	IntArray *arr = (IntArray *) malloc(sizeof(IntArray));
	AppAssert(arr);

	int_array_init(arr,size);

	return arr;
}

IntArray *int_array_copy(const IntArray *arr)
{
	IntArray *cpy;
	size_t i = 0;
	
	if(!arr) return 0;

	cpy = int_array_new(arr->size);
	
	cpy->lastindex = arr->lastindex;

	for(; i < arr->lastindex; ++i) cpy->data[i] = arr->data[i];

	return cpy;
}

void int_array_copy_inplace(IntArray *to, const IntArray *arr)
{
	size_t i = 0;
	if(!to || !arr) return;

	if(to->size < arr->size) int_array_grow(to,arr->lastindex);

	to->lastindex = arr->lastindex;

	for(; i < arr->lastindex; ++i) to->data[i] = arr->data[i];
}

void int_array_init(IntArray *arr,size_t size)
{
	if(size) {
		arr->data = (int *) malloc(sizeof(int) * size);
		AppAssert(arr->data);

		memset(arr->data,0,size);
	} else {
		arr->data = 0;
	}

	arr->size = size;
	arr->lastindex = 0;
}

void int_array_free(IntArray *arr)
{
	if(arr) {
		free(arr->data);
		free(arr);
	}
}

void int_array_empty(IntArray *arr)
{
	arr->lastindex = 0;
}

void int_array_empty_full(IntArray *arr)
{
	arr->lastindex = 0;
	free(arr->data);
	arr->data = 0;
	arr->size = 0;
}

size_t int_array_append(IntArray *arr,int data)
{
	int_array_insert(arr,data,arr->lastindex);
	return arr->lastindex-1;
}

void int_array_insert(IntArray *arr, int data, size_t index)
{
	if(index >= arr->size) int_array_grow(arr,index+1);
	arr->data[index] = data;
	arr->lastindex = max(index+1,arr->lastindex);
}

void int_array_move(IntArray *arr, size_t start, size_t end, int space)
{
	if(!arr || start > end || !int_array_valid(arr,start) || !space) return;

	if(end > arr->lastindex) end = arr->lastindex;

	if(space > 0) {
		int i;
		int li = (int)end + space - 1;

		if(li > arr->size) int_array_grow(arr,li);

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

		if(arr->lastindex <= (arr->size >> 2)) int_array_shrink(arr);

		arr->lastindex = max(end,arr->lastindex);
	}

}

void *int_array_foreach(IntArray *arr, TIterFunc func,void *user_data)
{
	size_t i = 0;
	for(; i < arr->lastindex; ++i) {
		void *value = func(&arr->data[i],user_data);
		if(value) return value;
	}

	return 0;
}

int int_array_pop(IntArray *arr,size_t index)
{
	int data;

	if(index >= arr->lastindex) return 0;

	data = arr->data[index];

	for(; index < arr->lastindex-1; ++index) arr->data[index] = arr->data[index+1];
	arr->lastindex -= 1;

	if(arr->lastindex <= (arr->size >> 2)) int_array_shrink(arr);

	return data;
}

void int_array_sort(IntArray *arr)
{
	//TODO
}

void int_array_remove(IntArray *arr,size_t index)
{
	if(index >= arr->lastindex) return;

	for(; index < arr->lastindex-1; ++index) arr->data[index] = arr->data[index+1];
	arr->lastindex -= 1;

	if(arr->lastindex <= (arr->size >> 2)) int_array_shrink(arr);
}

void int_array_remove_fast(IntArray *arr,size_t index)
{
	if(index >= arr->lastindex) return;

	arr->data[index] = arr->data[arr->lastindex-1];
	arr->data[arr->lastindex-1] = 0;
	arr->lastindex -= 1;

	if(arr->lastindex <= (arr->size >> 2)) int_array_shrink(arr);
}
