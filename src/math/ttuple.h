
#ifndef _included_terra_tuple_h
#define _included_terra_tuple_h

// Common tuples

typedef struct { short x, y; } Tuple2s;
typedef struct { int   x, y; } Tuple2i;
typedef struct { float x, y; } Tuple2f;

typedef struct { int   x, y, z; } Tuple3i;
typedef struct { float x, y, z; } Tuple3f;

typedef struct { int   x, y, z, t; } Tuple4i;
typedef struct { float x, y, z, t; } Tuple4f;

// common naming

typedef Tuple2f Point;
typedef Tuple2f Vector2f;

typedef Tuple3f Point3;
typedef Tuple3f Vector;

typedef Tuple4f Quat;

// 2D Point operation

inline static void PointAdd(Point *to, const Point *from)
{
	to->x += from->x;
	to->y += from->y;
}

inline static void PointSub(Point *to, const Point *from)
{
	to->x -= from->x;
	to->y -= from->y;
}

inline static unsigned char PointEqual(const Point *p1, const Point *p2)
{
	return p1->x == p2->x && p1->y == p2->y;
}

inline static float PointEuclidianDistance(const Point *to, const Point *from)
{
	float y = to->y-from->y, x = to->x-from->x;
	return sqrtf(y*y + x*x);
}

// 3-Tuple operations

inline static void Tuple3fSet(Tuple3f *t, float x, float y, float z)
{
	t->x = x; t->y = y; t->z = z;
}

inline static void Tuple3fCopy(Tuple3f *t1, const Tuple3f *t2)
{
	t1->x = t2->x; t1->y = t2->y; t1->z = t2->z;
}

inline static unsigned char Tuple3fEqual(const Tuple3f *t1,const Tuple3f *t2)
{
	return t1->x == t2->x && t1->y == t2->y &&t1->z == t2->z;
}

inline static void Tuple3fMin(Tuple3f *t1,const Tuple3f *t2)
{
	t1->x = TMIN(t1->x,t2->x);
	t1->y = TMIN(t1->y,t2->y);
	t1->z = TMIN(t1->z,t2->z);
}

inline static void Tuple3fMax(Tuple3f *t1,const Tuple3f *t2)
{
	t1->x = TMAX(t1->x,t2->x);
	t1->y = TMAX(t1->y,t2->y);
	t1->z = TMAX(t1->z,t2->z);
}

inline static void Tuple3fAdd(Tuple3f *t1,const Tuple3f *t2)
{
	t1->x += t2->x;
	t1->y += t2->y;
	t1->z += t2->z;
}

inline static void Tuple3fSub(Tuple3f *t1,const Tuple3f *t2)
{
	t1->x -= t2->x;
	t1->y -= t2->y;
	t1->z -= t2->z;
}

inline static void Tuple3fRotate(Vector *vector, const Tuple3f *cosangles, const Tuple3f *sinangles)
{
	float cx = cosangles->x, sx = sinangles->x;
	float cy = cosangles->y, sy = sinangles->y;
	float cz = cosangles->z, sz = sinangles->z;
	float x = vector->x;
	float y = vector->y;
	float z = vector->z;

	//rotate around x
	float t = y;

	y = cx*y - sx*z;
	z = sx*t + cx*z;

	//rotate around y
	t = x;

	x = cy*x + sy*z;
	vector->z = cy*z - sy*t;

	//rotate around z
	t = x;

	vector->x = cz*x - sz*y;
	vector->y = sz*t + cz*y;
}

// Vector operations

#define VectorSet Tuple3fSet
#define VectorRotate Tuple3fRotate

inline static Vector VectorCross(const Vector *v1, const Vector *v2)
{
	Vector result = {
		(v1->y * v2->z) - (v1->z * v2->y),
		(v1->z * v2->x) - (v1->x * v2->z),
		(v1->x * v2->y) - (v1->y * v2->x)
	};

	return result;
}

inline static float VectorDot(const Vector *v1, const Vector *v2)
{
	return (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z);
}

inline static float VectorLengthSquared(const Vector *v)
{
	return (v->x * v->x) + (v->y * v->y) + (v->z * v->z);
}

inline static float VectorLength(const Vector *v)
{
	return sqrtf(VectorLengthSquared(v));
}

inline static void VectorNormalize(Vector *v)
{
	float length = VectorLength(v);

	v->x /= length;
	v->y /= length;
	v->z /= length;
}

inline static Vector VectorNormalfromTuples(const Tuple3f *v1,const Tuple3f *v2,const Tuple3f *v3)
{
	Vector vec1 = *v1,vec2 = *v2,out;

	Tuple3fSub(&vec1,v2);
	Tuple3fSub(&vec2,v3);

	out = VectorCross(&vec1,&vec2);
	VectorNormalize(&out);

	return out;
}

#endif
