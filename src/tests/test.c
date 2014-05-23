
#include "stdafx.h"

#include "terra.h"

#include "array_test.h"
#include "rbtree_test.h"
#include "memleak_test.h"

int main(int argc, char **argv)
{
	TLogSetFile(0);

	TLogReport(T_LOG_PROGRESS,0,"Running tests...\n");

	//array_test();

	//rbtree_test();

	memleak_test();

	TLogReport(T_LOG_PROGRESS,0,"All Tests have been executed.\n");

	return 0;
}
