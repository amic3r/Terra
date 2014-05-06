#include "stdafx.h"

#include "structure/Heap.h"

#include "heap_test.h"

static Heap *test = 0;

char *strarr[] = {"bleh","blah","blih","bluh","blyh"};

void heap_test_init(void)
{
	test = heap_new(0);
	AppAssert(test);
}

void heap_test_init2(void)
{
	test = heap_new(1);
	AppAssert(test);
}

void heap_test_push(void)
{
	heap_push(test,10,strarr[0]);
	heap_push(test,5,strarr[1]);
	heap_push(test,2,strarr[2]);
	heap_push(test,12,strarr[3]);
	heap_push(test,7,strarr[4]);

	AppAssert(heap_num_elements(test) == 5);
}

static void dataprint(void *data)
{
	printf((const char *) data);
}

void heap_test_print(void)
{
	heap_print(test,dataprint);
}

void heap_test_pop(void)
{
	AppAssert(heap_pop(test) == strarr[3]);
	AppAssert(heap_pop(test) == strarr[0]);
	AppAssert(heap_pop(test) == strarr[4]);
	AppAssert(heap_pop(test) == strarr[1]);
	AppAssert(heap_pop(test) == strarr[2]);
}

void heap_test_pop2(void)
{
	AppAssert(heap_pop(test) == strarr[2]);
	AppAssert(heap_pop(test) == strarr[1]);
	AppAssert(heap_pop(test) == strarr[4]);
	AppAssert(heap_pop(test) == strarr[0]);
	AppAssert(heap_pop(test) == strarr[3]);
}

void heap_test_free(void)
{
	heap_free(test,0);
}

void heap_test(void)
{
	heap_test_init();

	heap_test_push();

	heap_test_print();

	heap_test_pop();

	heap_test_free();

	heap_test_init2();

	heap_test_push();

	heap_test_print();

	heap_test_pop2();

	heap_test_free();
}
