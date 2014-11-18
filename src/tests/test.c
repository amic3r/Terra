
#include "stdafx.h"

#include "test_utils.h"

#include "memleak_test.h"

#include "array_test.h"
#include "stack_test.h"
#include "rbtree_test.h"

#include "string_test.h"
#include "hardware_test.h"

#include "filesys_test.h"

#include "argparse_test.h"

int main(int argc, char **argv)
{
	TLogSetFile(stdout);

	TLogReport(T_LOG_PROGRESS,0,"Running tests...\n");

	//array_test();

	//rbtree_test();

	//memleak_test();

	//stack_test();

	//hardware_test();

	argparse_test();

	//string_test();

	//filesys_test();

	TLogReport(T_LOG_PROGRESS,0,"All Tests have been executed.\n");

	return 0;
}
