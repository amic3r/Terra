
#include "stdafx.h"

#include "targparser.h"

#include "talloc.h"
#include "terror.h"

#include "structure/tarray.h"

#include "tstring.h"

typedef struct {
	const char *name;

	char type;
	char amount;
	char def;

	const char *help;

	unsigned char used : 1;
} TArg;

typedef struct {
	void *param;
	size_t remainingParams;
	char paramtype;
	char paramdef;
} TParamHolder;

static struct {
	TArray *argFormat;

	TParamHolder ph;

	const char **argv;
	int argc;

	int idx;
} TArgParser;

void TArgParserInit(void)
{
	memset(&TArgParser,0, sizeof(TArgParser));
}

void TArgParserDestroy(void)
{
	TArrayFree(TArgParser.argFormat,TDeAlloc);
	TDeAlloc(TArgParser.ph.param);
}

void TArgParserAddArgument(const char *name, char type, char amount, char def, const char *help)
{
	TArg *arg = (TArg *) TAlloc(sizeof(TArg));
	if(arg) {
		arg->name = name;
		arg->type = type;
		arg->amount = amount;
		arg->def = def;
		arg->help = help;
		arg->used = 0;

		if(!TArgParser.argFormat) TArgParser.argFormat = TArrayNew(1);

		TArrayAppend(TArgParser.argFormat, arg);
	}
}

void TArgParserHelp(void)
{
	//TODO
	fprintf(stdout,
		"");
}

void TArgParserFeed(int argc, const char **argv)
{
	TArgParser.argc = argc;
	TArgParser.argv = argv;
}

static inline unsigned char TArgParserIsSwitch(const char *arg)
{
	return (*arg == '-');
}

static TArg *TArgParserMatchSwitch(const char *pattern)
{
	TArg *c = 0;
	void **data;
	size_t len, idx = 0;

	if(!TArgParser.argFormat) return 0;
	data = TArgParser.argFormat->data;
	len = TArgParser.argFormat->len;
	
	while (idx < len) {
		c = (TArg *) data[idx++];
		if(c->name && !strcmp(c->name,pattern))
			return c;
	}

	return 0;
}

static TArg *TArgParserMatchNonSwitch(void)
{
	TArg *c = 0;
	void **data;
	size_t len, idx = 0;

	if(!TArgParser.argFormat) return 0;
	data = TArgParser.argFormat->data;
	len = TArgParser.argFormat->len;
	
	while (idx < len) {
		c = (TArg *) data[idx];
		if(c->name && !TArgParserIsSwitch(c->name))
			return c;
	}

	return 0;
}

static size_t TArgParserNumData(int idx,size_t limit)
{
	int peek = idx;
	size_t amount = 0;

	while(peek < TArgParser.argc) {
		if(TArgParserIsSwitch(TArgParser.argv[peek++])) break;
		amount++;
		if(amount >= limit) break;
	}

	return amount;
}

static size_t TArgParserAmountLimit(const char format)
{
	if(!format || format == '0') {
		return 0;
	} else if(format == '?') {
		return 1;
	} else if(format > '0' && format <= '9') {
		return format - '0';
	}

	return TArgParser.argc - TArgParser.idx;
}

static unsigned char TArgParserIsAmountValid(const char format, size_t amount)
{
	size_t limit = TArgParserAmountLimit(format);

	if(format >= '0' && format <= '9')
		return amount == limit;

	return amount <= limit;
}

static unsigned char TArgParserIsTypeValid(const char type, const char *arg)
{
	if(type == 'd' || type == 'i') {
		if(*arg == '-') arg++;

		for(;*arg;arg++)
			if (*arg < '0' || *arg > '9') return 0;
	} else if(type == 'u') {
		for(;*arg;arg++)
			if (*arg < '0' || *arg > '9') return 0;
	} else if(type == 'x') {
		for(;*arg;arg++)
			if ((*arg < '0' || *arg > '9') && (*arg < 'a' || *arg > 'f')) return 0;
	} else if(type == 'X') {
		for(;*arg;arg++)
			if ((*arg < '0' || *arg > '9') && (*arg < 'A' || *arg > 'F')) return 0;
	} else if(type == 'f' || type == 'F') {
		unsigned char hasdot = 0;
		for(;*arg;arg++)  {
			if ((*arg < '0' || *arg > '9') && (*arg != '.' || hasdot)) return 0;
			if(*arg == '.') hasdot = 1;
		}
	} else if(type == 'c') {
		return *arg && !*(arg+1);
	}

	return 1;
}

static unsigned char TArgParserCheck(TArg *pat, const char *arg)
{
	size_t i = 0,ndata;
	size_t limit;

	if(!pat) return 1;

	// check if the pattern was already used

	if(pat->used) {
		TErrorReportFull(1,"Pattern for switch already used.");
		TArgParserHelp();
		return 1;
	}
	pat->used = 1;

	// check if the amount of data is correct
	limit = TArgParserAmountLimit(pat->amount);

	ndata = TArgParserNumData(TArgParser.idx, limit);
	if(!TArgParserIsAmountValid(pat->amount,ndata)) {
		TErrorReportFull(1,"Invalid number of arguments.");
		return 1;
	}

	// check if the type is correct
	for(; i < ndata; ++i) {
		arg = TArgParser.argv[TArgParser.idx + i];
		if(!TArgParserIsTypeValid(pat->type,arg)) {
			TErrorReportFull(1,"Invalid argument.");
			return 1;
		}
	}

	return 0;
}

static inline void TArgParserStoreParam(char type, char def, size_t amount)
{
	TDeAlloc(TArgParser.ph.param);
	TArgParser.ph.param = 0;
	TArgParser.ph.paramtype = type;
	TArgParser.ph.paramdef = def;
	TArgParser.ph.remainingParams = amount;
}

const char *TArgParserNext(void)
{
	const char *next = 0;

	// maybe we have reached the end already
	if(!TArgParser.argv || (TArgParser.idx >= TArgParser.argc)) {
		if(TArgParser.ph.param) {
			TDeAlloc(TArgParser.ph.param);
			TArgParser.ph.param = 0;
		}
		return next;
	}

	if(TArgParser.ph.remainingParams) {
		TArgParser.idx += TArgParser.ph.remainingParams;
		TArgParserStoreParam(0,0,0);
	}

	while((!next) && (TArgParser.idx < TArgParser.argc)) {
		TArg *pat;

		next = TArgParser.argv[TArgParser.idx];
		if(!next) {
			TErrorReport(2);
			return 0;
		}

		if(TArgParserIsSwitch(next)) {
			//argument is a switch
			//find matching pattern
			pat = TArgParserMatchSwitch(next);
			TArgParser.idx++;
		} else {
			pat = TArgParserMatchNonSwitch();
		}

		if(TArgParserCheck(pat,next)) return 0;

		{
			// keep parameters ready
			size_t limit = TArgParserAmountLimit(pat->amount);
			size_t amount = TArgParserNumData(TArgParser.idx, limit);
			if(amount) TArgParserStoreParam(pat->type,pat->def,amount);
		}
	}

	return next;
}

const void *TArgParserNextParameter(void)
{
	const void *data;
	const char *arg;
	char type = TArgParser.ph.paramtype;

	if(!TArgParser.ph.remainingParams) return 0;

	TDeAlloc(TArgParser.ph.param);

	arg = TArgParser.argv[TArgParser.idx];

	if(type == 'd' || type == 'i') {
		int *p;
		TArgParser.ph.param = TAlloc(sizeof(int));
		p = (int *) TArgParser.ph.param;
		
		sscanf(arg,"%d",p);
		data = TArgParser.ph.param;
	} else if(type == 'u') {
		size_t *p;
		TArgParser.ph.param = TAlloc(sizeof(size_t));
		p = (size_t *) TArgParser.ph.param;
		
		sscanf(arg,"%u",p);
		data = TArgParser.ph.param;
	} else if(type == 'x' || type == 'X') {
		unsigned long *p;
		TArgParser.ph.param = TAlloc(sizeof(unsigned long));
		p = (unsigned long *) TArgParser.ph.param;

		sscanf(arg, type == 'x' ? "%x" : "%X",p);
		data = TArgParser.ph.param;
	} else if(type == 'f' || type == 'F') {
		double *p;
		TArgParser.ph.param = TAlloc(sizeof(double));
		p = (double *) TArgParser.ph.param;

		sscanf(arg, "%lf",p);
		data = TArgParser.ph.param;
	} else if(type == 'c') {
		char *p;
		TArgParser.ph.param = TAlloc(sizeof(char));
		p = (char *) TArgParser.ph.param;

		sscanf(arg, "%c",p);
		data = TArgParser.ph.param;
	} else {
		data = arg;
		TArgParser.ph.param = 0;
	}

	TArgParser.ph.remainingParams--;
	TArgParser.idx++;
	return data;
}
