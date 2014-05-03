
#ifndef __included_terra_define
#define __included_terra_define

typedef void (*TFreeFunc) (void *);
typedef void *(*TIterFunc) (void *,void *);

typedef int (*TCompareFunc) (void *,void *);

#endif
