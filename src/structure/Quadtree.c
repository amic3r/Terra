
#include "stdafx.h"

#include "SList.h"
#include "tmath.h"

#include "quadtree.h"

QuadPt *quadpt_new(const Point *p, void *data)
{
	QuadPt *pt = (QuadPt *) malloc(sizeof(QuadPt));
	AppAssert(pt);
	pt->pt = *p;
	pt->data = data;

	return pt;
}

typedef struct _QuadNode {
	struct _QuadNode *nodes[4];
	TRectangle bounds;
	SList *pts;
} QuadNode;

void quadnode_init(QuadNode *q,const TRectangle *bounds)
{
	AppAssert(q);

	memset(q->nodes,0, sizeof(QuadNode *) * 4);
	q->bounds = *bounds;
	q->pts = slist_new();
}


QuadNode *quadnode_new(const TRectangle *bounds)
{
	QuadNode *qn = (QuadNode *) malloc(sizeof(QuadNode));
	quadnode_init(qn,bounds);
	return qn;
}

void quadnode_free(QuadNode *q,void (*data_free)(void *))
{
	if(q) {
		if(q->nodes[0]) {
			quadnode_free(q->nodes[0],data_free);
			quadnode_free(q->nodes[1],data_free);
			quadnode_free(q->nodes[2],data_free);
			quadnode_free(q->nodes[3],data_free);
		} else if(data_free) {
			QuadPt *pt = (QuadPt *) slist_pop(q->pts,0);
			while(pt) {
				data_free(pt->data);
				free(pt);
				pt = (QuadPt *) slist_pop(q->pts,0);
			}
		} else {
			slist_empty(q->pts,free);
		}
		free(q);
	}
}

void quadnode_split(QuadNode *q)
{
	TRectangle bounds = {q->bounds.x,q->bounds.y,q->bounds.w/2,q->bounds.h/2};
	QuadNode *s;
	QuadPt *pt;

	q->nodes[0] = quadnode_new(&bounds); bounds.x += bounds.w;
	q->nodes[1] = quadnode_new(&bounds); bounds.x = q->bounds.x; bounds.y += bounds.h;
	q->nodes[2] = quadnode_new(&bounds); bounds.x += bounds.w;
	q->nodes[3] = quadnode_new(&bounds);

	pt = (QuadPt *) slist_pop(q->pts,0);
	while(pt) {
		Point *p = &pt->pt;
		if(terra_rectangle_contains_point(&q->nodes[0]->bounds,(int)p->x,(int)p->y)) s = q->nodes[0];
		else if(terra_rectangle_contains_point(&q->nodes[1]->bounds,(int)p->x,(int)p->y)) s = q->nodes[1];
		else if(terra_rectangle_contains_point(&q->nodes[2]->bounds,(int)p->x,(int)p->y)) s = q->nodes[2];
		else s = q->nodes[3];

		slist_append(s->pts,pt);

		pt = (QuadPt *) slist_pop(q->pts,0);
	};

	slist_free(q->pts,free);
	q->pts = 0;
}

void quadnode_insert(QuadNode *q,const Point *p, void *data, int limit, int levellimit)
{
	int level = 1;
	QuadPt *pt;

	if(!terra_rectangle_contains_point(&q->bounds,p->x,p->y)) return;

	pt = quadpt_new(p,data);

	while(q) {
		if(q->nodes[0]) {
			if(terra_rectangle_contains_point(&q->nodes[0]->bounds,p->x,p->y)) q = q->nodes[0];
			else if(terra_rectangle_contains_point(&q->nodes[1]->bounds,p->x,p->y)) q = q->nodes[1];
			else if(terra_rectangle_contains_point(&q->nodes[2]->bounds,p->x,p->y)) q = q->nodes[2];
			else q = q->nodes[3];
			level++;
		} else {
			slist_append(q->pts,pt);
			if(q->pts->len >= limit && level < levellimit) quadnode_split(q);
			q = 0;
		}
	}
}

void quadnode_remove(QuadNode *q,const Point *pt, void (*data_free)(void *))
{
	if(!terra_rectangle_contains_point(&q->bounds,pt->x,pt->y)) return;

	while(q) {
		if(q->nodes[0]) {
			if(terra_rectangle_contains_point(&q->nodes[0]->bounds,pt->x,pt->y)) q = q->nodes[0];
			else if(terra_rectangle_contains_point(&q->nodes[1]->bounds,pt->x,pt->y)) q = q->nodes[1];
			else if(terra_rectangle_contains_point(&q->nodes[2]->bounds,pt->x,pt->y)) q = q->nodes[2];
			else q = q->nodes[3];
		} else {
			QuadPt *c = (QuadPt *) slist_first(q->pts);
			while(c) {
				if(c->pt.x == pt->x && c->pt.y == pt->y) {
					slist_remove(q->pts,c);
					if(data_free) data_free(c->data);
					free(c);
					break;
				}
				c = (QuadPt *) slist_next(q->pts);
			}

			q = 0;
		}
	}
}

const SList *quadnode_fetch(const QuadNode *q, int x, int y)
{
	const SList *found = 0;

	if(!terra_rectangle_contains_point(&q->bounds,x,y)) return 0;

	while(q) {
		if(q->nodes[0]) {
			if(terra_rectangle_contains_point(&q->nodes[0]->bounds,x,y)) q = q->nodes[0];
			else if(terra_rectangle_contains_point(&q->nodes[1]->bounds,x,y)) q = q->nodes[1];
			else if(terra_rectangle_contains_point(&q->nodes[2]->bounds,x,y)) q = q->nodes[2];
			else q = q->nodes[3];
		} else {
			found = q->pts;
			q = 0;
		}
	}

	return found;
}

SList *quadnode_fetch_all(const QuadNode *q,const TRectangle *rect)
{
	SList *found, nodes;

	if(!terra_rectangle_contains(&q->bounds,rect)) return 0;

	found = slist_new();
	slist_init(&nodes);

	do {
		if(q->nodes[0]) {
			if(terra_rectangle_contains(&q->nodes[0]->bounds,rect)) slist_append(&nodes,q->nodes[0]);
			if(terra_rectangle_contains(&q->nodes[1]->bounds,rect)) slist_append(&nodes,q->nodes[1]);
			if(terra_rectangle_contains(&q->nodes[2]->bounds,rect)) slist_append(&nodes,q->nodes[2]);
			if(terra_rectangle_contains(&q->nodes[3]->bounds,rect)) slist_append(&nodes,q->nodes[3]);
		} else {
			Point *p = (Point *) slist_first(q->pts);
			while(p) {
				if(terra_rectangle_contains_point(rect,p->x,p->y)) slist_append(found,p);
				p = (Point *) slist_next(q->pts);
			}
		}

		q = (QuadNode *) slist_pop(&nodes,0);
	} while(q);

	return found;
}

SList *quadnode_fetch_near(const QuadNode *q,const Point *p, float dist)
{
	// find all data near the point
	TRectangle rect = {p->x-dist,p->y-dist,dist*2,dist*2};
	SList *found, nodes;

	if(!terra_rectangle_contains_point(&q->bounds,p->x,p->y)) return 0;

	found = slist_new();
	slist_init(&nodes);
	
	do {
		if(q->nodes[0]) {
			if(terra_rectangle_contains(&q->nodes[0]->bounds,&rect)) slist_append(&nodes,q->nodes[0]);
			if(terra_rectangle_contains(&q->nodes[1]->bounds,&rect)) slist_append(&nodes,q->nodes[1]);
			if(terra_rectangle_contains(&q->nodes[2]->bounds,&rect)) slist_append(&nodes,q->nodes[2]);
			if(terra_rectangle_contains(&q->nodes[3]->bounds,&rect)) slist_append(&nodes,q->nodes[3]);
		} else {
			Point *p2 = (Point *) slist_first(q->pts);
			while(p2) {
				if(euclidian_distance(p2,p) < dist) slist_append(found,p2);
				p2 = (Point *) slist_next(q->pts);
			}
		}

		q = (QuadNode *) slist_pop(&nodes,0);
	} while(q);

	return found;
}

struct _QuadTree {
	QuadNode *head;
	size_t obj_limit, level_limit;
};

QuadTree *quadtree_new(const TRectangle *bounds)
{
	QuadTree *qt = (QuadTree *) malloc(sizeof(QuadTree));

	qt->level_limit = 5;
	qt->obj_limit = 50;

	qt->head = quadnode_new(bounds);

	return qt;
}

void quadtree_set(QuadTree *qt, size_t obj_limit, size_t level_limit)
{
	AppAssert(qt);

	qt->level_limit = level_limit;
	qt->obj_limit = obj_limit;
}

void quadtree_empty(QuadTree *qt, void (*data_free)(void *))
{
	TRectangle bounds = qt->head->bounds;
	quadnode_free(qt->head,data_free);

	qt->head = quadnode_new(&bounds);
}

void quadtree_free(QuadTree *qt, void (*data_free)(void *))
{
	if(qt) {
		quadnode_free(qt->head,data_free);
		free(qt);
	}
}

void quadtree_insert(QuadTree *qt,const Point *pt,void *data) {
	AppAssert(qt);

	quadnode_insert(qt->head,pt,data,qt->obj_limit,qt->level_limit);
}

const SList *quadtree_fetch(const QuadTree *qt, int x, int y) {
	AppAssert(qt);

	return quadnode_fetch(qt->head,x,y);
}

SList *quadtree_fetch_all(const QuadTree *qt,const TRectangle *rect) {
	AppAssert(qt);

	return quadnode_fetch_all(qt->head,rect);
}

SList *quadtree_fetch_near(const QuadTree *qt,const Point *p, float dist) {
	AppAssert(qt);

	return quadnode_fetch_near(qt->head,p,dist);
}

void quadtree_remove(QuadTree *qt,const Point *pt, void (*data_free)(void *))
{
	AppAssert(qt);

	quadnode_remove(qt->head,pt,data_free);
}
