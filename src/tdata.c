
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

TData TDataNewCpy(const void *data, size_t size, char type)
{
	TData content = (TData) TAlloc(sizeof(TData));

	char *d = (char *) TAlloc(size);
	memcpy(d,data,size);

	content->data = d;
	
	content->type = type;
	content->autodelete = 1;
	content->autocast = 1;

	return content;
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
	return TDataNewCpy(data,size,T_DATA_UNKNOWN);
}

TData TDataFromChar(char data)
{
	return TDataNewCpy(&data,sizeof(char),T_DATA_CHAR);
}

TData TDataFromInt(int data)
{
	return TDataNewCpy(&data,sizeof(int),T_DATA_INT32);
}

TData TDataFromUnsignedInt(unsigned int data)
{
	return TDataNewCpy(&data,sizeof(int),T_DATA_UINT32);
}

TData TDataFromString(const char *data)
{
	return TDataNewCpy(&data,sizeof(char) * (strlen(data) + 1),T_DATA_STRING);
}

TData TDataFromFloat(float data)
{
	return TDataNewCpy(&data,sizeof(float),T_DATA_FLOAT);
}

TData TDataFromDouble(double data)
{
	return TDataNewCpy(&data,sizeof(double),T_DATA_DOUBLE);
}

void TDataSetAutoCast(TData data, char cast)
{
	data->autocast = cast;
}

char TDataGetType(TData data)
{
	return data->type;
}

const void *TDataToPointer(TData data, char *type)
{
	if(type) *type = data->type;
	return data->data;
}

char TDataToChar(TData data)
{
	if(data) {
		if(data->type == T_DATA_CHAR) {
			return *((char *) data->data);
		} //TODO casting
	}

	TErrorReport(5);
	return 0;
}

int TDataToInt(TData data)
{
	if(data) {
		if(data->type == T_DATA_INT32) {
			return *((int *) data->data);
		} //TODO casting
	}

	TErrorReport(5);
	return 0;
}

unsigned int TDataToUnsignedInt(TData data)
{
	if(data) {
		if(data->type == T_DATA_UINT32) {
			return *((unsigned int *) data->data);
		} //TODO casting
	}

	TErrorReport(5);
	return 0;
}

const char *TDataToString(TData data)
{
	if(data) {
		if(data->type == T_DATA_STRING) {
			return (char *) data->data;
		} //TODO casting
	}

	TErrorReport(5);
	return 0;
}

float TDataToFloat(TData data)
{
	if(data) {
		if(data->type == T_DATA_FLOAT) {
			return *((float *) data->data);
		} //TODO casting
	}

	TErrorReport(5);
	return 0;
}

double TDataToDouble(TData data)
{
	if(data) {
		if(data->type == T_DATA_DOUBLE) {
			return *((double *) data->data);
		} //TODO casting
	}

	TErrorReport(5);
	return 0;
}

void TDataFree(TData data)
{
	if(data) {
		if(data->autodelete) TDeAlloc(data->data);
		TDeAlloc(data);
	}
}

#ifdef __cplusplus
}
#endif
