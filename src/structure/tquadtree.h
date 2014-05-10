
#ifndef __included_terra_auadtree_h
#define __included_terra_TQuadTree_h

#include "tdefine.h"

#define TERRA_RECTANGLE
#include "tmath.h"
#undef TERRA_RECTANGLE

#include "tlist.h"

typedef struct {
	Point pt;
	void *data;
} TQuadPt;

typedef struct _TQuadTree TQuadTree;

TQuadTree *TQuadTreeNew(const TRectangle *bounds);

void TQuadTreeSet(TQuadTree *qt, size_t obj_limit, size_t level_limit);

void TQuadTreeEmpty(TQuadTree *qt, TFreeFunc func);
void TQuadTreeFree(TQuadTree *qt, TFreeFunc func);

void TQuadTreeInsert(TQuadTree *qt,const Point *pt,void *data);

const TSList *TQuadTreeFetch(const TQuadTree *qt, const Point *p);
TSList *TQuadTreeFetchAll(const TQuadTree *qt,const TRectangle *rect);
TSList *TQuadTreeFetchNear(const TQuadTree *qt,const Point *p, float dist);

void TQuadTreeRemove(TQuadTree *qt,const Point *pt, TFreeFunc func);

#endif
