
#ifndef __included_terra_error_h
#define __included_terra_error_h

void TErrorReport(size_t code);
void TErrorReportFull(size_t code, const char *message);

int TErrorGetCode(void);
const char *TErrorGetString(void);

void TErrorClear(void);

#define TError(code) \
	TErrorReport(code); \
	return

#define TErrorFull(code, message) \
	TErrorReportFull(code, message); \
	return

#endif
