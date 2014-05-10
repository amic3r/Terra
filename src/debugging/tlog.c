
#include "stdafx.h"

#include "tlog.h"

static FILE *tLogFile = 0;

const char *cattotext[] = {"","Warning: ","Error: "};

void TLogSetFile(FILE *_file)
{
	tLogFile = _file;
	if(!tLogFile) {
		TLogReport(T_LOG_WARNING,"TLogSetFile","The file pointer used for logging is null. stdout will be used instead.");
		tLogFile = stdout;
	}
}

void TLogReport(int category, const char *function, const char *format,...)
{
	va_list ap;

	TLogStartReport(category,function);

	va_start(ap, format);
	vfprintf(tLogFile,format,ap);
	va_end(ap);

	fprintf(tLogFile,"\n");
}

void TLogStartReport(int category, const char *function)
{
	fprintf(tLogFile,cattotext[category]);
	if(category > 0) fprintf(tLogFile,"In function %s: ", function);
}

void TLogWrite(const char *format,...)
{
	va_list ap;

	va_start(ap, format);
	vfprintf(tLogFile,format,ap);
	va_end(ap);
}

void TLogWriteV(const char *format,va_list ap)
{
	vfprintf(tLogFile,format,ap);
}
