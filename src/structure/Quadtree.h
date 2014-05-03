
#ifndef __included_terra_quadtree_h
#define __included_terra_quadtree_h

typedef struct {
	Point pt;
	void *data;
} QuadPt;

typedef struct _QuadTree QuadTree;

QuadTree *quadtree_new(const TRectangle *bounds);

void quadtree_set(QuadTree *qt, size_t obj_limit, size_t level_limit);

void quadtree_empty(QuadTree *qt, void (*data_free)(void *));
void quadtree_free(QuadTree *qt, void (*data_free)(void *));

void quadtree_insert(QuadTree *qt,const Point *pt,void *data);

const SList *quadtree_fetch(const QuadTree *qt, int x, int y);
SList *quadtree_fetch_all(const QuadTree *qt,const TRectangle *rect);
SList *quadtree_fetch_near(const QuadTree *qt,const Point *p, float dist);

void quadtree_remove(QuadTree *qt,const Point *pt, void (*data_free)(void *));

#endif
