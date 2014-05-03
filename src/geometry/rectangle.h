
#ifndef _included_terra_rectangle_h
#define _included_terra_rectangle_h

#include "stdafx.h"

typedef struct {
	int x,y;
	int w,h;
} TRectangle;

static inline void terra_rectangle_set(TRectangle *r,int x, int y, unsigned int w, unsigned int h)
{
	r->x = x;
	r->y = y;
	r->w = w;
	r->h = h;
}

static inline void terra_rectangle_cpy(TRectangle *r1,const TRectangle *r2)
{
	r1->x = r2->x;
	r1->y = r2->y;
	r1->w = r2->w;
	r1->h = r2->h;
}

static inline unsigned char terra_rectangle_intersect(const TRectangle *r1, const TRectangle *r2)
{
	return (r1->x+r1->w >= r2->x || r1->x <= r2->x+r2->w) &&
		   (r1->y+r1->h >= r2->y || r1->y <= r2->y+r2->h);
}

static inline unsigned char terra_rectangle_contains(const TRectangle *r1, const TRectangle *r2)
{
	return (r1->x+r1->w >= r2->x+r2->w && r1->x <= r2->x) &&
		   (r1->y+r1->h >= r2->y+r2->h && r1->y <= r2->y);
}

static inline unsigned char terra_rectangle_contains_point(const TRectangle *r, int x, int y)
{
	return (r->x+r->w >= x && r->x <= x) &&
		   (r->y+r->h >= y && r->y <= y);
}

static inline unsigned char terra_rectangle_equal(const TRectangle *r1, const TRectangle *r2)
{
	return r2->x == r1->x && r2->y == r1->y && r2->w == r1->w && r2->h == r1->h;
}

static inline void terra_rectangle_move(TRectangle *r, int x, int y)
{
	r->x = x;
	r->y = y;
}

static inline void terra_rectangle_resize(TRectangle *r, unsigned int w, unsigned int h)
{
	r->w = w;
	r->h = h;
}

static inline void terra_rectangle_merge(TRectangle *r1, const TRectangle *r2)
{
	if(r2->x == -1) return;

	if (r1->x == -1) {
		r1->x = r2->x;
		r1->w = r2->w;
	} else {
		int x2 = r1->x + r1->w;
		r1->x = min(r1->x,r2->x);
		r1->w = max(x2,r2->x + r2->w) - r1->x;
	}

	if (r1->y == -1) {
		r1->y = r2->y;
		r1->h = r2->h;
	} else {
		int y2 = r1->y + r1->h;
		r1->y = min(r1->y,r2->y);
		r1->h = max(y2,r2->y + r2->h) - r1->y;
	}
}

#endif
