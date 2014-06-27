
#include "stdafx.h"

#include "tquadtree.h"

#include "talloc.h"

#include "tarray.h"
#include "tstack.h"

#include "utility/tinteger.h"

TQuadObj *TQuadObjNew(const TRectangle *rect, void *data)
{
	TQuadObj *pt = (TQuadObj *) TAlloc(sizeof(TQuadObj));
	if(!pt) return 0;

	pt->rect = *rect;
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
		}
		
		if(func) {
			TQuadObj *obj = (TQuadObj *) TSListPop(q->pts);
			while(obj) {
				func(obj->data);
				free(obj);
				obj = (TQuadObj *) TSListPop(q->pts);
			}
		}
		TSListFree(q->pts,free);

		free(q);
	}
}

static inline int findObjPosition(const TQuadNode **cur, const TQuadNode *head, const TRectangle *r)
{
	size_t i = 0;
	const TQuadNode *c = 0;
	size_t level = 0;

	while(head) {
		if(!head->nodes[0]) {
			c = head;
			break;
		}

		for(; i < 4; ++i) {
			if(TRectangleContains(&head->nodes[i]->bounds,r)) {
				if (c) { c = head; break; }
				else c = head->nodes[i];
			}
		}

		if(c == head) break;
		head = c;
		level++;
	}

	*cur = c;

	return level;
}

void TQuadNodeSplit(TQuadNode *q)
{
	TRectangle bounds = {q->bounds.x,q->bounds.y,q->bounds.w/2,q->bounds.h/2};
	const TQuadNode *s;
	TQuadObj *obj;
	size_t size = q->pts->len;

	q->nodes[0] = TQuadNodeNew(&bounds); bounds.x += bounds.w;
	q->nodes[1] = TQuadNodeNew(&bounds); bounds.x = q->bounds.x; bounds.y += bounds.h;
	q->nodes[2] = TQuadNodeNew(&bounds); bounds.x += bounds.w;
	q->nodes[3] = TQuadNodeNew(&bounds);

	obj = (TQuadObj *) TSListPop(q->pts);
	while(size--) {
		findObjPosition(&s,q,&obj->rect);

		TSListPrepend(s->pts,obj);

		obj = (TQuadObj *) TSListPop(q->pts);
	};
}

void TQuadNodeInsert(TQuadNode *q,TQuadObj *obj, size_t limit, size_t levellimit)
{
	size_t level = 1;
	const TQuadNode *s;

	if(!TRectangleContains(&q->bounds,&obj->rect)) return;

	level += findObjPosition(&s,q,&obj->rect);

	TSListAppend(q->pts,obj);
	if(q->pts->len >= limit && level < levellimit) TQuadNodeSplit(q);
}

void TQuadNodeRemove(TQuadNode *q,size_t id, const TRectangle *r, TFreeFunc func)
{
	TQuadObj *obj;

	if(!TRectangleContains(&q->bounds,r)) return;

	findObjPosition((const TQuadNode **) &q,q,r);

	obj = (TQuadObj *) TSListFirst(q->pts);
	while(obj) {
		if(obj->id == id) {
			TSListRemove(q->pts,obj);
			if(func) func(obj->data);
			free(obj);
			break;
		}
		obj = (TQuadObj *) TSListNext(q->pts);
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
			TQuadObj *p = (TQuadObj *) TSListFirst(q->pts);
			while(p) {
				if(TRectangleContains(rect,&p->rect)) TSListAppend(found,p);
				p = (TQuadObj *) TSListNext(q->pts);
			}
		}

		q = (TQuadNode *) TSListPopIndex(&nodes,0);
	} while(q);

	return found;
}

struct _TQuadTree {
	TQuadNode *head;
	size_t obj_limit, level_limit;
	TArray objects;
	TStack *freespace;
};

TQuadTree *quadtree_new(const TRectangle *bounds)
{
	TQuadTree *qt = (TQuadTree *) TAlloc(sizeof(TQuadTree));
	if(!qt) return 0;

	qt->level_limit = 5;
	qt->obj_limit = 50;
	TArrayInit(&qt->objects,0);
	qt->freespace = TStackNew();

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

	TArrayEmptyFull(&qt->objects,0);
	TStackEmpty(qt->freespace,free);

	qt->head = TQuadNodeNew(&bounds);
}

void TQuadTreeFree(TQuadTree *qt, TFreeFunc func)
{
	if(qt) {
		TQuadNodeFree(qt->head,func);
		TArrayEmptyFull(&qt->objects,0);
		TStackFree(qt->freespace,free);
		free(qt);
	}
}

size_t TQuadTreeInsert(TQuadTree *qt,const TRectangle *rect,void *data) {
	if(qt) {
		TQuadObj *obj = TQuadObjNew(rect,data);
		int *position = (int *) TStackPop(qt->freespace);

		TQuadNodeInsert(qt->head,obj,qt->obj_limit,qt->level_limit);

		if(position) {
			obj->id = *position;
			TArrayInsert(&qt->objects,obj,obj->id);

			free(position);
		} else {
			obj->id = TArrayAppend(&qt->objects,obj);
		}
	}

	return 0;
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

void TQuadTreeRemove(TQuadTree *qt,size_t id, TFreeFunc func)
{
	if(qt) {
		if(TArrayValid(&qt->objects,id)) {
			const TQuadObj *obj = (const TQuadObj *) qt->objects.data[id];

			if(obj) {
				TQuadNodeRemove(qt->head,id,&obj->rect,func);
				TStackPush(qt->freespace,TIntegerToPtr(id));
			}
		}
	}
}
