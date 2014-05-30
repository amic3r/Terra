#include "stdafx.h"

#include "debugging/tdebug.h"
#include "utility/tstring.h"

#include "string_test.h"

void string_test_split(void)
{
	const char *path = "test/bleh/";
	size_t size;
	char **split = TStringSplit(path,"/",&size);

	TAssert(split);
	TAssert(size == 3);
	TAssert(!strcmp(split[size-1], ""));

	free(split[0]);
	free(split);
}

void string_test(void)
{
	TLogReport(T_LOG_PROGRESS,0,"Testing string utilities...\n");

	string_test_split();

	TLogReport(T_LOG_PROGRESS,0,"string utilities tests completed.\n");
}
