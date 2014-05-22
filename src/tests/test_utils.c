#include "stdafx.h"

#include "test_utils.h"

#include "debugging/tdebug.h"

void test_create_file(const char *filename, const char *data)
{
	FILE *f = fopen(filename,"w");

	if(!f) TAbort("Binary Stream Reader Test : can't open test file for writing");

	fwrite(data,strlen(data),1,f);

	fclose(f);
}
