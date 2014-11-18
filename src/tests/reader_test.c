#include "stdafx.h"

#include "io/treader.h"
#include "utility/tfilesys.h"

#include "test_utils.h"

#include "reader_test.h"

void test_reader()
{
	const char data[] = "a 42 this is        a test\n";
	const char filename[] = "test_file_for_testing";
	const char *token = 0;
	TReader *testread = 0;
	unsigned char character = 0;
	unsigned char buffer[32];

	test_create_file(filename,data);

	testread = TReaderFileNew(filename,0);
	TAssert(testread);

	TAssert(!TReaderEof(testread));
	TAssert(TReaderSize(testread) == strlen(data)+1);

	character = TReaderRead8(testread);
	TAssert(character == 'a');

	TReaderReadBlock(testread,32,buffer);

	TReaderSeek(testread,0,0);
	
	token = TReaderNextToken(testread);
	while(token)
		token = TReaderNextToken(testread);

	TReaderFree(testread,1);
	TFileSysDelete(filename);
}
