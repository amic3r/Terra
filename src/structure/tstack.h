
#ifndef __included_terra_stack_h
#define __included_terra_stack_h

typedef struct TStack *TStack;

TStack TStackNew(void);
void TStackFree(TStack stack, TFreeFunc func);

void TStackEmpty(TStack stack, TFreeFunc func);

void TStackPush(TStack stack, void *data);
void *TStackPop(TStack stack);

void *TStackPeek(TStack stack);

void TStackResize(TStack stack,size_t _size);

size_t TStackCount(TStack stack);

#endif
