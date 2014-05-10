
#include "stdafx.h"

#include "tquadtree.h"

#include "talloc.h"

TQuadPt *TQuadPtNew(const Point *p, void *data)
{
	TQuadPt *pt = (TQuadPt *) TAlloc(sizeof(TQuadPt));
	if(!pt) return 0;

	pt->pt = *p;
	pt->data = data;
	return pt;
}

typedef struct _TQuadNode {
	struct _TQuadNode *nodes[4];
	TRectangle bounds;
	TSList *pts;
} TQuadNode;

void TQuadNodeInit(TQuadNode *q,const TRectangle *bounds)
{
	memset(q->nodes,0, sizeof(TQuadNode *) * 4);
	q->bounds = *bounds;
	q->pts = TSListNew();
}


TQuadNode *TQuadNodeNew(const TRectangle *bounds)
{
	TQuadNode *qn = (TQuadNode *) TAlloc(sizeof(TQuadNode));
	if(qn) TQuadNodeInit(qn,bounds);
	return qn;
}

void TQuadNodeFree(TQuadNode *q,TFreeFunc func)
{
	if(q) {
		if(q->nodes[0]) {
			TQuadNodeFree(q->nodes[0],func);
			TQuadNodeFree(q->nodes[1],func);
			TQuadNodeFree(q->nodes[2],func);
			TQuadNodeFree(q->nodes[3],func);
		} else if(func) {
			TQuadPt *pt = (TQuadPt *) TSListPop(q->pts);
			while(pt) {
				func(pt->data);
				free(pt);
				pt = (TQuadPt *) TSListPop(q->pts);
			}
		} else {
			TSListEmpty(q->pts,free);
		}
		free(q);
	}
}

void TQuadNodeSplit(TQuadNode *q)
{
	TRectangle bounds = {q->bounds.x,q->bounds.y,q->bounds.w/2,q->bounds.h/2};
	TQuadNode *s;
	TQuadPt *pt;

	q->nodes[0] = TQuadNodeNew(&bounds); bounds.x += bounds.w;
	q->nodes[1] = TQuadNodeNew(&bounds); bounds.x = q->bounds.x; bounds.y += bounds.h;
	q->nodes[2] = TQuadNodeNew(&bounds); bounds.x += bounds.w;
	q->nodes[3] = TQuadNodeNew(&bounds);

	pt = (TQuadPt *) TSListPop(q->pts);
	while(pt) {
		Point *p = &pt->pt;
		if(TRectangleContainsPointF(&q->nodes[0]->bounds,p)) s = q->nodes[0];
		else if(TRectangleContainsPointF(&q->nodes[1]->bounds,p)) s = q->nodes[1];
		else if(TRectangleContainsPointF(&q->nodes[2]->bounds,p)) s = q->nodes[2];
		else s = q->nodes[3];

		TSListAppend(s->pts,pt);

		pt = (TQuadPt *) TSListPop(q->pts);
	};

	TSListFree(q->pts,free);
	q->pts = 0;
}

void TQuadNodeInsert(TQuadNode *q,const Point *p, void *data, size_t limit, size_t levellimit)
{
	size_t level = 1;
	TQuadPt *pt;

	if(!TRectangleContainsPointF(&q->bounds,p)) return;

	pt = TQuadPtNew(p,data);

	while(q) {
		if(q->nodes[0]) {
			if(TRectangleContainsPointF(&q->nodes[0]->bounds,p)) q = q->nodes[0];
			else if(TRectangleContainsPointF(&q->nodes[1]->bounds,p)) q = q->nodes[1];
			else if(TRectangleContainsPointF(&q->nodes[2]->bounds,p)) q = q->nodes[2];
			else q = q->nodes[3];
			level++;
		} else {
			TSListAppend(q->pts,pt);
			if(q->pts->len >= limit && level < levellimit) TQuadNodeSplit(q);
			q = 0;
		}
	}
}

void TQuadNodeRemove(TQuadNode *q,const Point *pt, TFreeFunc func)
{
	if(!TRectangleContainsPointF(&q->bounds,pt)) return;

	while(q) {
		if(q->nodes[0]) {
			if(TRectangleContainsPointF(&q->nodes[0]->bounds,pt)) q = q->nodes[0];
			else if(TRectangleContainsPointF(&q->nodes[1]->bounds,pt)) q = q->nodes[1];
			else if(TRectangleContainsPointF(&q->nodes[2]->bounds,pt)) q = q->nodes[2];
			else q = q->nodes[3];
		} else {
			TQuadPt *c = (TQuadPt *) TSListFirst(q->pts);
			while(c) {
				if(PointEqual(&c->pt,pt)) {
					TSListRemove(q->pts,c);
					if(func) func(c->data);
					free(c);
					break;
				}
				c = (TQuadPt *) TSListNext(q->pts);
			}

			q = 0;
		}
	}
}

const TSList *TQuadNodeFetch(const TQuadNode *q, const Point *p)
{
	const TSList *found = 0;

	if(!TRectangleContainsPointF(&q->bounds,p)) return 0;

	while(q) {
		if(q->nodes[0]) {
			if(TRectangleContainsPointF(&q->nodes[0]->bounds,p)) q = q->nodes[0];
			else if(TRectangleContainsPointF(&q->nodes[1]->bounds,p)) q = q->nodes[1];
			else if(TRectangleContainsPointF(&q->nodes[2]->bounds,p)) q = q->nodes[2];
			else q = q->nodes[3];
		} else {
			found = q->pts;
			q = 0;
		}
	}

	return found;
}

TSList *TQuadNodeFetchAll(const TQuadNode *q,const TRectangle *rect)
{
	TSList *found, nodes;

	if(!TRectangleContains(&q->bounds,rect)) return 0;

	found = TSListNew();
	TSListInit(&nodes);

	do {
		if(q->nodes[0]) {
			if(TRectangleContains(&q->nodes[0]->bounds,rect)) TSListAppend(&nodes,q->nodes[0]);
			if(TRectangleContains(&q->nodes[1]->bounds,rect)) TSListAppend(&nodes,q->nodes[1]);
			if(TRectangleContains(&q->nodes[2]->bounds,rect)) TSListAppend(&nodes,q->nodes[2]);
			if(TRectangleContains(&q->nodes[3]->bounds,rect)) TSListAppend(&nodes,q->nodes[3]);
		} else {
			Point *p = (Point *) TSListFirst(q->pts);
			while(p) {
				if(TRectangleContainsPointF(rect,p)) TSListAppend(found,p);
				p = (Point *) TSListNext(q->pts);
			}
		}

		q = (TQuadNode *) TSListPopIndex(&nodes,0);
	} while(q);

	return found;
}

struct _TQuadTree {
	TQuadNode *head;
	size_t obj_limit, level_limit;
};

TQuadTree *quadtree_new(const TRectangle *bounds)
{
	TQuadTree *qt = (TQuadTree *) TAlloc(sizeof(TQuadTree));
	if(!qt) return 0;

	qt->level_limit = 5;
	qt->obj_limit = 50;

	qt->head = TQuadNodeNew(bounds);

	return qt;
}

void TQuadTreeSet(TQuadTree *qt, size_t obj_limit, size_t level_limit)
{
	if(qt) {
		qt->level_limit = level_limit;
		qt->obj_limit = obj_limit;
	}
}

void TQuadTreeEmpty(TQuadTree *qt, TFreeFunc func)
{
	TRectangle bounds = qt->head->bounds;
	TQuadNodeFree(qt->head,func);

	qt->head = TQuadNodeNew(&bounds);
}

void TQuadTreeFree(TQuadTree *qt, TFreeFunc func)
{
	if(qt) {
		TQuadNodeFree(qt->head,func);
		free(qt);
	}
}

void TQuadTreeInsert(TQuadTree *qt,const Point *pt,void *data) {
	if(qt) TQuadNodeInsert(qt->head,pt,data,qt->obj_limit,qt->level_limit);
}

const TSList *TQuadTreeFetch(const TQuadTree *qt, const Point *p) {
	if(qt) return TQuadNodeFetch(qt->head,p);

	return 0;
}

TSList *TQuadTreeFetchAll(const TQuadTree *qt,const TRectangle *rect) {
	if(qt) return TQuadNodeFetchAll(qt->head,rect);

	return 0;
}

TSList *TQuadTreeFetchNear(const TQuadTree *qt,const Point *p, float dist) {
	if(qt) {
		TRectangle rect = {(int) (p->x-dist),(int) (p->y-dist),(int) (dist*2),(int) (dist*2)};
		return TQuadNodeFetchAll(qt->head,&rect);
	}

	return 0;
}

void TQuadTreeRemove(TQuadTree *qt,const Point *pt, TFreeFunc func)
{
	if(qt) TQuadNodeRemove(qt->head,pt,func);
}
