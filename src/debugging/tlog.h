
#ifndef __included_tlog_h
#define __included_tlog_h

enum T_LOG_EVENT {
	T_LOG_PROGRESS = 0,
	T_LOG_WARNING,
	T_LOG_ERROR,
};

void tlog_set_file(FILE *_file);

void tlog_report(int category, const char *format,...);

#endif
