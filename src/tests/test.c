
#include "stdafx.h"

#include "terra.h"

#include "array_test.h"

int main(int argc, char **argv)
{
	tlog_report(T_LOG_PROGRESS,"Running tests...\n");

	array_test();

	tlog_report(T_LOG_PROGRESS,"All Tests have been executed.\n");
	
	return 0;
}
