#include "stdafx.h"

#include "terror.h"

static int TErrorCode = 0;
static const char *TErrorString = 0;

static const char *TErrorCommon[] = {
	"No error",
	"Unknown error",
	"Invalid Input",
	"Out of memory",
	"Permission denied",
};
static size_t TErrorAmount = 4;

void TErrorReport(size_t code)
{
	TErrorReportFull(code, code < TErrorAmount ? TErrorCommon[code] : TErrorCommon[0]);
}

void TErrorReportFull(size_t code, const char *message)
{
	TErrorClear();

	TErrorCode = code;
	TErrorString = message;
}

int TErrorGetCode(void)
{
	return TErrorCode;
}

const char *TErrorGetString(void)
{
	return TErrorString;
}

void TErrorClear(void)
{
	TErrorString = 0;
	TErrorCode = 0;
}
