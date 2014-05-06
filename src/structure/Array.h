
#ifndef __included_terra_array_h
#define __included_terra_array_h

/** 
  * Dynamic Array
  */

typedef struct _Array {
	void **data;
	size_t size,used;
	size_t lastindex;
} Array;

Array *array_new(size_t size);

Array *array_copy(const Array *arr, void *(*data_cpy)(void *));
void array_copy_inplace(Array *to, const Array *arr, void *(*data_cpy)(void *));

void array_init(Array *arr,size_t size);

void array_free(Array *arr,TFreeFunc func);

void array_empty(Array *arr,TFreeFunc func);
void array_empty_full(Array *arr,TFreeFunc func);

size_t array_append(Array *arr,void *data);
void array_insert(Array *arr,void *data, size_t index);

void array_move(Array *arr, size_t start, size_t end, int space);

void *array_foreach(Array *arr, void *(*func)(void *,void *),void *user_data);

static inline unsigned char array_valid(Array *arr,size_t index)
{
	return index < arr->lastindex;
}

static inline void *array_get(Array *arr,size_t index)
{
	if(!array_valid(arr,index)) return 0;
	return arr->data[index];
}

#define array_push array_append
void *array_pop(Array *arr,size_t index);

void array_sort(Array *arr);

void array_remove(Array *arr,size_t index);
void array_remove_fast(Array *arr,size_t index); // grab end element and put it in place
void array_remove_clear(Array *arr,size_t index); // replace with 0

//------------- Integer Array ---------------//

/** 
  * Dynamic Integer Array
  */

typedef struct _IntArray {
	int *data;
	size_t size;
	size_t lastindex;
} IntArray;

IntArray *int_array_new(size_t size);

IntArray *int_array_copy(const IntArray *arr);
void int_array_copy_inplace(IntArray *to, const IntArray *arr);

void int_array_init(IntArray *arr,size_t size);

void int_array_free(IntArray *arr);

void int_array_empty(IntArray *arr);
void int_array_empty_full(IntArray *arr);

size_t int_array_append(IntArray *arr,int data);
void int_array_insert(IntArray *arr,int data, size_t index);

void *int_array_foreach(IntArray *arr, TIterFunc func,void *user_data);

static inline unsigned char int_array_valid(IntArray *arr,size_t index)
{
	return index < arr->lastindex;
}

inline int int_array_get(IntArray *arr,size_t index)
{
	if(!int_array_valid(arr,index)) return 0;
	return arr->data[index];
}

int int_array_pop(IntArray *arr,size_t index);

void int_array_sort(IntArray *arr);

void int_array_remove(IntArray *arr,size_t index);
void int_array_remove_fast(IntArray *arr,size_t index); // grab end element and put it in place

#define int_array_push int_array_append

#endif
