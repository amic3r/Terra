
#include "stdafx.h"

#include "terra.h"

#include "array_test.h"

int main(int argc, char **argv)
{
	TLogReport(T_LOG_PROGRESS,0,"Running tests...\n");

	array_test();

	TLogReport(T_LOG_PROGRESS,0,"All Tests have been executed.\n");
	
	return 0;
}
