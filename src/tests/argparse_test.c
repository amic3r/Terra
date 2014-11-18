
#include "stdafx.h"

#include "test_utils.h"

void argparse_arguments(void)
{
	TArgParserAddArgument("input",'c','1','z',0);
	TArgParserAddArgument("-p",'s','1',0,0);
	TArgParserAddArgument("--math",0,'0',0,0);
}

void argparse_feed(void)
{
	static const char *content[] = {
		"Terra.exe",
		"-p",
		"bleh",
		"a",
		"--math"
	};
	TArgParserFeed(4,content+1);
}

void argparse_run(void)
{
	const void *data;
	const char *out = 0;

	while((out = TArgParserNext())) {
		if(!strcmp(out,"--math")) {
			data = TArgParserNextParameter();
			TAssert(!data);
		} else if (!strcmp(out,"-p")) {
			data = TArgParserNextParameter();
			TAssert(data);
			TAssert(!strcmp("bleh",(const char *) data));
		} else if (out) {
			printf("%s\n",out);
		}
	}
	printf("%s",TErrorGetString());
}

void argparse_test(void)
{
	TArgParserInit();

	argparse_arguments();
	argparse_feed();

	argparse_run();

	TArgParserDestroy();
}
