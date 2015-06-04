
#include "stdafx.h"

#include "tlog.h"

static FILE *tLogFile = 0;

const char *cattotext[] = {"","Warning: ","Error: "};

void TLogSetFile(FILE *_file)
{
	tLogFile = _file;
}

void TLogReport(int category, const char *function, const char *format,...)
{
	if(tLogFile) {
		va_list ap;

		TLogStartReport(category,function);

		va_start(ap, format);
		vfprintf(tLogFile,format,ap);
		va_end(ap);

		fprintf(tLogFile,"\n");

		fflush(tLogFile);
	}
}

void TLogStartReport(int category, const char *function)
{
	if(tLogFile) {
		fputs(cattotext[category],tLogFile);
		if(category > 0) fprintf(tLogFile,"In function %s: ", function);
		fflush(tLogFile);
	}
}

void TLogWrite(const char *format,...)
{
	if(tLogFile) {
		va_list ap;

		va_start(ap, format);
		vfprintf(tLogFile,format,ap);
		va_end(ap);

		fflush(tLogFile);
	}
}

void TLogWriteV(const char *format,va_list ap)
{
	if(tLogFile) {
		vfprintf(tLogFile,format,ap);
		fflush(tLogFile);
	}
}
