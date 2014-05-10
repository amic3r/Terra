
#ifndef __included_tlog_h
#define __included_tlog_h

enum T_LOG_EVENT {
	T_LOG_PROGRESS = 0,
	T_LOG_WARNING,
	T_LOG_ERROR,
};

void TLogSetFile(FILE *_file);

void TLogReport(int category, const char *function, const char *format,...);

void TLogStartReport(int category, const char *function);

void TLogWrite(const char *format,...);
void TLogWriteV(const char *format,va_list ap);

#endif
