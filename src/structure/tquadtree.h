
#ifndef __included_terra_auadtree_h
#define __included_terra_TQuadTree_h

#include "tdefine.h"

#define TERRA_RECTANGLE
#include "tmath.h"

#include "tlist.h"

typedef struct {
	TRectangle rect;
	void *data;
} TQuadObj;

typedef struct _TQuadTree TQuadTree;

TQuadTree *TQuadTreeNew(const TRectangle *bounds);

void TQuadTreeSet(TQuadTree *qt, size_t obj_limit, size_t level_limit);

void TQuadTreeEmpty(TQuadTree *qt, TFreeFunc func);
void TQuadTreeFree(TQuadTree *qt, TFreeFunc func);

TQuadObj *TQuadTreeInsert(TQuadTree *qt,const TRectangle *rect,void *data);

void TQuadTreeUpdate(const TQuadTree *qt,TQuadObj *obj, const TRectangle *newposition);
void TQuadTreeBringToFront(const TQuadTree *qt,const TQuadObj *obj);
void TQuadTreePushToBack(const TQuadTree *qt,const TQuadObj *obj);

const TSList *TQuadTreeFetch(const TQuadTree *qt, const Point *p);
TSList *TQuadTreeFetchAll(const TQuadTree *qt,const TRectangle *rect);
TSList *TQuadTreeFetchNear(const TQuadTree *qt,const Point *p, float dist);

void TQuadTreeRemove(TQuadTree *qt,TQuadObj *obj, TFreeFunc func);

#endif
