#include "stdafx.h"

#include "math.h"

Line *line_new(void)
{
	return line_newset(0.0f,0.0f,0.0f,0.0f);
}

Line *line_newset(float x1, float y1, float x2, float y2)
{
	Line *l = (Line *) malloc(sizeof(Line));
	AppAssert(l);

	l->x1 = x1; l->y1 = y1;
	l->x2 = x2; l->y2 = y2;

	return l;
}

void line_set(Line *l,float x1, float y1, float x2, float y2)
{
	AppAssert(l);

	l->x1 = x1; l->y1 = y1;
	l->x2 = x2; l->y2 = y2;
}

unsigned char line_intersect(const Line *l1,const Line *l2, float *x, float *y)
{
	float a1,b1,c1;
	float a2,b2,c2;
	float det;

	AppAssert(l1 && l2);

	a1 = l1->y2-l1->y1;
	b1 = l1->x1-l1->x2;
	c1 = a1*l1->x1 + b1*l1->y1;

	a2 = l2->y2-l2->y1;
	b2 = l2->x1-l2->x2;
	c2 = a2*l2->x1 + b2*l2->y1;

	det = a1*b2 - a2*b1;

	
	if((int)det == 0) return 0;

	if(x) *x = (b2*c1 - b1*c2)/det;
	if(y) *y = (a1*c2 - a2*c1)/det;

	return 1;
}
