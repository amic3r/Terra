#include "stdafx.h"

#include "debugging/tlog.h"
#include "structure/Array.h"
#include "utility/integer.h"

#include "array_test.h"

static Array testarray;
static IntArray iarr;
static char *yay = "yay";

void array_test_init(void)
{
	array_init(&testarray,0);
}

void array_test_insert(void)
{
	array_append(&testarray,integer_to_ptr(1));

	AppAssert(*((int *) array_get(&testarray,0)) == 1);

	array_insert(&testarray,integer_to_ptr(5),4);

	AppAssert(*((int *) array_get(&testarray,4)) == 5);
	AppAssert(array_get(&testarray,2) == 0);
}

void array_test_move(void)
{
	//array_move(&testarray,0,5,-2);
}

static void *test_loop_1(void *data,void *udata)
{
	if(data) {
		*((int *) udata) = *((int *) data);
		printf("%d",*((int *) data));
	}
	else printf("<null>");

	return 0;
}

static void *test_loop_2(void *data,void *udata)
{
	if(data && *((int *) data) == 5) return yay;

	return 0;
}

void array_test_foreach(void)
{
	int temp;

	printf("test array content : ");
	array_foreach(&testarray,test_loop_1,&temp);

	AppAssert(temp == 5);

	AppAssert(array_foreach(&testarray,test_loop_2,0) == yay);
}

void array_test_remove(void)
{
	array_remove(&testarray,0);

	AppAssert(*((int *) array_get(&testarray,3)) == 5);

	array_insert(&testarray,integer_to_ptr(1),0);

	array_remove_fast(&testarray,0);

	AppAssert(*((int *) array_get(&testarray,0)) == 5);

	array_remove_clear(&testarray,0);

	AppAssert(!array_get(&testarray,0));

	AppAssert(!array_pop(&testarray,0));

	array_remove(&testarray,0);
}

void array_test_free(void)
{
	array_empty_full(&testarray,free);
}

static void *intarr_print(int *v, void *udata)
{
	printf("%d",*v);

	return 0;
}

void array_test(void)
{
	tlog_report(T_LOG_PROGRESS,"Testing array...\n");

	array_test_init();

	array_test_insert();

	array_test_move();

	array_test_foreach();

	array_test_remove();

	array_test_free();

	tlog_report(T_LOG_PROGRESS,"Array tests completed.\n");
}
