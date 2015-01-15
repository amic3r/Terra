
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

TData TDataFromMem(void *data, size_t size);
TData TDataFromConstMem(const void *data);
TData TDataFromChar(char data);
TData TDataFromInt(int data);
TData TDataFromUnsignedInt(unsigned int data);
TData TDataFromString(const char *data);
TData TDataFromFloat(float data);
TData TDataFromDouble(double data);

void TDataSetFromMem(TData context, void *data);
void TDataSetFromConstMem(TData context, const void *data, size_t size);
void TDataSetFromChar(TData context, char data);
void TDataSetFromInt(TData context, int data);
void TDataSetFromUnsignedInt(TData context, unsigned int data);
void TDataSetFromString(TData context, const char *data);
void TDataSetFromFloat(TData context, float data);
void TDataSetFromDouble(TData context, double data);

void TDataSetAutoCast(TData context, char cast);

char TDataGetType(TData context);

const void *TDataToPointer(TData context, char *type);
char TDataToChar(TData context);
int TDataToInt(TData context);
unsigned int TDataToUnsignedInt(TData context);
const char *TDataToString(TData context);
float TDataToFloat(TData context);
double TDataToDouble(TData context);

void TDataEmpty(TData context);
void TDataFree(TData context);

#endif
