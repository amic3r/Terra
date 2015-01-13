
#include "stdafx.h"

#include "tdata.h"
#include "talloc.h"
#include "terror.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _TData {
	char *data;
	size_t size;

	char type;

	char autodelete : 1;
	char autocast : 1;
};

static inline void TDataCpySet(TData context, const void *data, size_t size, char type)
{
	char *d = (char *) TAlloc(size);
	memcpy(d,data,size);

	context->data = d;
	
	context->type = type;
	context->autodelete = 1;
	context->autocast = 1;
}

static inline TData TDataNewCpy(const void *data, size_t size, char type)
{
	TData context = (TData) TAlloc(sizeof(TData));
	TDataCpySet(context, data, size, type);
	return context;
}

TData TDataFromMem(void *data)
{
	TData content = (TData) TAlloc(sizeof(TData));

	content->data = (char *)data;
	content->type = T_DATA_UNKNOWN;
	content->autodelete = 0;
	content->autocast = 0;

	return content;
}

TData TDataFromConstMem(const void *data, size_t size)
{
	return TDataNewCpy(data, size, T_DATA_UNKNOWN);
}

TData TDataFromChar(char data)
{
	return TDataNewCpy(&data, sizeof(char), T_DATA_CHAR);
}

TData TDataFromInt(int data)
{
	return TDataNewCpy(&data, sizeof(int), T_DATA_INT32);
}

TData TDataFromUnsignedInt(unsigned int data)
{
	return TDataNewCpy(&data, sizeof(int), T_DATA_UINT32);
}

TData TDataFromString(const char *data)
{
	return TDataNewCpy(&data, sizeof(char) * (strlen(data) + 1), T_DATA_STRING);
}

TData TDataFromFloat(float data)
{
	return TDataNewCpy(&data, sizeof(float), T_DATA_FLOAT);
}

TData TDataFromDouble(double data)
{
	return TDataNewCpy(&data, sizeof(double), T_DATA_DOUBLE);
}

void TDataSetFromMem(TData context, void *data)
{
	TDataEmpty(context);

	context->data = (char *)data;
}

void TDataSetFromConstMem(TData context, const void *data, size_t size)
{
	TDataEmpty(context);
	TDataCpySet(context, data, size, T_DATA_UNKNOWN);
}

void TDataSetFromChar(TData context, char data)
{
	TDataEmpty(context);
	TDataCpySet(context, &data, sizeof(char), T_DATA_CHAR);
}

void TDataSetFromInt(TData context, int data)
{
	TDataEmpty(context);
	TDataCpySet(context, &data, sizeof(int), T_DATA_INT32);
}

void TDataSetFromUnsignedInt(TData context, unsigned int data)
{
	TDataEmpty(context);
	TDataCpySet(context, &data, sizeof(int), T_DATA_UINT32);
}

void TDataSetFromString(TData context, const char *data)
{
	TDataEmpty(context);
	TDataCpySet(context, &data, sizeof(char) * (strlen(data) + 1), T_DATA_STRING);
}

void TDataSetFromFloat(TData context, float data)
{
	TDataEmpty(context);
	TDataCpySet(context, &data, sizeof(float), T_DATA_FLOAT);
}

void TDataSetFromDouble(TData context, double data)
{
	TDataEmpty(context);
	TDataCpySet(context, &data, sizeof(double), T_DATA_DOUBLE);
}

void TDataSetAutoCast(TData context, char cast)
{
	context->autocast = cast;
}

char TDataGetType(TData context)
{
	return context->type;
}

const void *TDataToPointer(TData context, char *type)
{
	if(type) *type = context->type;
	return context->data;
}

char TDataToChar(TData context)
{
	if(context) {
		if(context->type == T_DATA_CHAR) {
			return *((char *) context->data);
		} //TODO casting
	}

	TErrorReport(5);
	return 0;
}

int TDataToInt(TData context)
{
	if(context) {
		if(context->type == T_DATA_INT32) {
			return *((int *) context->data);
		} //TODO casting
	}

	TErrorReport(5);
	return 0;
}

unsigned int TDataToUnsignedInt(TData context)
{
	if(context) {
		if(context->type == T_DATA_UINT32) {
			return *((unsigned int *) context->data);
		} //TODO casting
	}

	TErrorReport(5);
	return 0;
}

const char *TDataToString(TData context)
{
	if(context) {
		if(context->type == T_DATA_STRING) {
			return (char *) context->data;
		} //TODO casting
	}

	TErrorReport(5);
	return 0;
}

float TDataToFloat(TData context)
{
	if(context) {
		if(context->type == T_DATA_FLOAT) {
			return *((float *) context->data);
		} //TODO casting
	}

	TErrorReport(5);
	return 0;
}

double TDataToDouble(TData context)
{
	if(context) {
		if(context->type == T_DATA_DOUBLE) {
			return *((double *) context->data);
		} //TODO casting
	}

	TErrorReport(5);
	return 0;
}

void TDataEmpty(TData context) {
	if(context) {
		if(context->autodelete) TDeAlloc(context->data);
		context->data = 0;
		context->type = T_DATA_UNKNOWN;
		context->autodelete = 0;
		context->autocast = 0;
	}
}

void TDataFree(TData context)
{
	if(context) {
		if(context->autodelete) TDeAlloc(context->data);
		TDeAlloc(context);
	}
}

#ifdef __cplusplus
}
#endif
