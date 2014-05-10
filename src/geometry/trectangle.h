
#ifndef _included_terra_rectangle_h
#define _included_terra_rectangle_h

#include "tdefine.h"

typedef struct {
	int x,y;
	int w,h;
} TRectangle;

static inline void TRectangleSet(TRectangle *r,int x, int y, unsigned int w, unsigned int h)
{
	r->x = x;
	r->y = y;
	r->w = w;
	r->h = h;
}

static inline void TRectangleCopy(TRectangle *r1,const TRectangle *r2)
{
	r1->x = r2->x;
	r1->y = r2->y;
	r1->w = r2->w;
	r1->h = r2->h;
}

static inline unsigned char TRectangleIntersect(const TRectangle *r1, const TRectangle *r2)
{
	return (r1->x+r1->w >= r2->x || r1->x <= r2->x+r2->w) &&
		   (r1->y+r1->h >= r2->y || r1->y <= r2->y+r2->h);
}

static inline unsigned char TRectangleContains(const TRectangle *r1, const TRectangle *r2)
{
	return (r1->x+r1->w >= r2->x+r2->w && r1->x <= r2->x) &&
		   (r1->y+r1->h >= r2->y+r2->h && r1->y <= r2->y);
}

static inline unsigned char TRectangleContainsPoint(const TRectangle *r, int x, int y)
{
	return (r->x+r->w >= x && r->x <= x) &&
		   (r->y+r->h >= y && r->y <= y);
}

static inline unsigned char TRectangleContainsPointF(const TRectangle *r, const Point *pt)
{
	float x1 = (float) r->x, y1 = (float) r->y;
	float x2 = (float) (r->x+r->w), y2 = (float)(r->y+r->h);
	return (x2 >= pt->x && x1 <= pt->x) &&
		   (y2 >= pt->y && y1 <= pt->y);
}

static inline unsigned char TRectangleEqual(const TRectangle *r1, const TRectangle *r2)
{
	return r2->x == r1->x && r2->y == r1->y && r2->w == r1->w && r2->h == r1->h;
}

static inline void TRectangleMove(TRectangle *r, int x, int y)
{
	r->x = x;
	r->y = y;
}

static inline void TRectangleResize(TRectangle *r, unsigned int w, unsigned int h)
{
	r->w = w;
	r->h = h;
}

static inline void TRectangleMerge(TRectangle *r1, const TRectangle *r2)
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
