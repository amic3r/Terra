
#include "stdafx.h"

#include "tlog.h"

static FILE *f = 0;

const char *cattotext[] = {"","Warning: ","Error: "};

void tlog_set_file(FILE *_file)
{
	f = _file;
	if(!f) tlog_report(T_LOG_WARNING,"The file pointer used for logging is null. stdout will be used instead.\n");
}

void tlog_report(int category, const char *format,...)
{
	va_list ap;

	va_start(ap, format);

	if(f) {
		fprintf(f,cattotext[category]);
		vfprintf(f,format,ap);
	} else {
		printf(cattotext[category]);
		vprintf(format,ap);
	}

	va_end(ap);
}
