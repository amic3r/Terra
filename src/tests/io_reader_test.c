#include "stdafx.h"

#include "eclipse.h"

#include "test_utils.h"

#include "io_reader_test.h"

void test_io_reader()
{
	const char data[] = "a 42 this is        a test\n";
	const char filename[] = "test_file_for_testing";
	const char *token = 0;
	IOReader *testread = 0;
	unsigned char character = 0;
	unsigned char buffer[32];

	test_create_file(filename,data);

	testread = file_reader_new(filename,0);
	AppAssert(testread);

	AppAssert(!io_reader_eof(testread));
	AppAssert(io_reader_size(testread) == strlen(data)+1);

	character = io_reader_read8(testread);
	AppAssert(character == 'a');

	io_reader_read_block(testread,32,buffer);

	io_reader_seek(testread,0,0);
	
	token = io_reader_next_token(testread);
	while(token)
		token = io_reader_next_token(testread);

	io_reader_free(testread,1);
	DeleteThisFile(filename);
}
