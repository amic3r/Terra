
#ifndef __included_terra_data_h
#define __included_terra_data_h

enum T_DATA_TYPE {
	T_DATA_UNKNOWN = 0,
	T_DATA_CHAR,
	T_DATA_INT32,
	T_DATA_UINT32,
	T_DATA_INT64,
	T_DATA_UINT64,
	T_DATA_STRING,
	T_DATA_FLOAT,
	T_DATA_DOUBLE,
};

typedef struct _TData * TData;

TData TDataFromMem(void *data);
TData TDataFromConstMem(const void *data, size_t size);

TData TDataFromChar(char data);
TData TDataFromInt(int data);
TData TDataFromUnsignedInt(unsigned int data);
TData TDataFromString(const char *data);
TData TDataFromFloat(float data);
TData TDataFromDouble(double data);

void TDataSetAutoCast(TData data, char cast);

char TDataGetType(TData data);

const void *TDataToPointer(TData data, char *type);
char TDataToChar(TData data);
int TDataToInt(TData data);
unsigned int TDataToUnsignedInt(TData data);
const char *TDataToString(TData data);
float TDataToFloat(TData data);
double TDataToDouble(TData data);

void TDataFree(TData data);

#endif
