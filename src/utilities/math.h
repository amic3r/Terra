
#ifndef _included_terra_math_h
#define _included_terra_math_h

#include <math.h>

//Default storage

typedef struct {
	int x, y;
} Tuple2i;

typedef struct {
	float x, y;
} Tuple2f;

typedef struct {
	int x, y, z;
} Tuple3i;

typedef struct {
	float x, y, z;
} Tuple3f;

typedef struct {
	float x, y, z, w;
} Tuple4f;

typedef union _Matrix3f{
	struct {
		union {float m00; float xx;};
		union {float m10; float xy;};
		union {float m20; float xz;};
		union {float m01; float yx;};
		union {float m11; float yy;};
		union {float m21; float yz;};
		union {float m02; float zx;};
		union {float m12; float zy;};
		union {float m22; float zz;};
	} s;
	float M[9];
} Matrix3f;

typedef union _Matrix4f {
	struct {
		union {float m00; float xx;};
		union {float m10; float xy;};
		union {float m20; float xz;};
		union {float m30; float xw;};
		union {float m01; float yx;};
		union {float m11; float yy;};
		union {float m21; float yz;};
		union {float m31; float yw;};
		union {float m02; float zx;};
		union {float m12; float zy;};
		union {float m22; float zz;};
		union {float m32; float zw;};
		union {float m03; float tx;};
		union {float m13; float ty;};
		union {float m23; float tz;};
		union {float m33; float tw;};
	} s;
	float M[16];
} Matrix4f;

#define Point  Tuple2f

#define Vector2f Tuple2f

#define Point3 Tuple3f
#define Vector Tuple3f

#define Quat   Tuple4f

// utility macros
// assuming IEEE-754(GLfloat), which i believe has max precision of 7 bits
#define Epsilon 1.0e-5

__inline static void PointAdd(Point *to, const Point *from)
{
	AppAssert(to && from);

	to->x += from->x;
	to->y += from->y;
}

__inline static void PointSub(Point *to, const Point *from)
{
	AppAssert(to && from);

	to->x -= from->x;
	to->y -= from->y;
}

__inline float euclidian_distance(const Point *to, const Point *from)
{
	float y = to->y-from->y, x = to->x-from->x;
	return sqrtf(y*y + x*x);
}

__inline void Tuple3fSet(Tuple3f *tuple, float x, float y, float z)
{
	tuple->x = x; tuple->y = y; tuple->z =  z;
}

__inline unsigned char Tuple3fEqual(const Tuple3f *t1,const Tuple3f *t2)
{
	return t1->x == t2->x && t1->y == t2->y &&t1->z == t2->z;
}

__inline void Tuple3fAdd(Tuple3f *t1,const Tuple3f *t2)
{
	t1->x += t2->x;
	t1->y += t2->y;
	t1->z += t2->z;
}

__inline void Tuple3fSub(Tuple3f *t1,const Tuple3f *t2)
{
	t1->x -= t2->x;
	t1->y -= t2->y;
	t1->z -= t2->z;
}

__inline void Tuple3fRotate(Vector *vector, const Tuple3f *cosangles, const Tuple3f *sinangles)
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

#define VectorSet Tuple3fSet
#define VectorRotate Tuple3fRotate

__inline static Vector VectorCross(const Vector *v1, const Vector *v2)
{
	Vector result;

	AppAssert(v1 && v2);

	result.x = (v1->y * v2->z) - (v1->z * v2->y);
	result.y = (v1->z * v2->x) - (v1->x * v2->z);
	result.z = (v1->x * v2->y) - (v1->y * v2->x);

	return result;
}

__inline static float VectorDot(const Vector *v1, const Vector *v2)
{
	AppAssert(v1 && v2);

	return (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z);
}

__inline static float VectorLengthSquared(const Vector *v)
{
	AppAssert(v);

	return (v->x * v->x) + (v->y * v->y) + (v->z * v->z);
}

__inline static float VectorLength(const Vector *v)
{
	AppAssert(v);

	return sqrtf(VectorLengthSquared(v));
}

__inline static void VectorNormalize(Vector *v)
{
	float length = VectorLength(v);

	v->x /= length;
	v->y /= length;
	v->z /= length;
}

__inline Vector NormalfromTuples(const Tuple3f *v1,const Tuple3f *v2,const Tuple3f *v3)
{
	Vector vec1,vec2,out;

	vec1.x = v1->x - v2->x;
	vec1.y = v1->y - v2->y;
	vec1.z = v1->z - v2->z;

	vec2.x = v2->x - v3->x;
	vec2.y = v2->y - v3->y;
	vec2.z = v2->z - v3->z;

	out = VectorCross(&vec1,&vec2);
	VectorNormalize(&out);

	return out;
}

__inline static void Matrix3fSetZero(Matrix3f *mat)
{
	mat->s.m00 = mat->s.m01 = mat->s.m02 = mat->s.m10 = mat->s.m11 = mat->s.m12 = mat->s.m20 = mat->s.m21 = mat->s.m22 = 0.0f;
}

__inline static void Matrix3fSetIdentity(Matrix3f * mat)
{
	Matrix3fSetZero(mat);

	mat->s.m00 = mat->s.m11 = mat->s.m22 = 1.0f;
}

__inline static void Matrix3fSetRotationFromQuat(Matrix3f *mat, const Quat * q1)
{
	float n, s;
	float xs, ys, zs;
	float wx, wy, wz;
	float xx, xy, xz;
	float yy, yz, zz;

	AppAssert(mat && q1);

	n = (q1->x * q1->x) + (q1->y * q1->y) + (q1->z * q1->z) + (q1->w * q1->w);
	s = (n > 0.0f) ? (2.0f / n) : 0.0f;

	xs = q1->x * s;  ys = q1->y * s;  zs = q1->z * s;
	wx = q1->w * xs; wy = q1->w * ys; wz = q1->w * zs;
	xx = q1->x * xs; xy = q1->x * ys; xz = q1->x * zs;
	yy = q1->y * ys; yz = q1->y * zs; zz = q1->z * zs;

	mat->s.xx = 1.0f - (yy + zz); mat->s.yx = xy - wz;          mat->s.zx = xz + wy;
	mat->s.xy = xy + wz;          mat->s.yy = 1.0f - (xx + zz); mat->s.zy = yz - wx;
	mat->s.zx = xz - wy;          mat->s.yz = yz + wx;          mat->s.zz = 1.0f - (xx + yy);
}

__inline static void Matrix3fMulMatrix3f(Matrix3f *m1, const Matrix3f * m2)
{
	AppAssert(m1 && m2);

	m1->s.m00 = (m1->s.m00 * m2->s.m00) + (m1->s.m01 * m2->s.m10) + (m1->s.m02 * m2->s.m20);
	m1->s.m01 = (m1->s.m00 * m2->s.m01) + (m1->s.m01 * m2->s.m11) + (m1->s.m02 * m2->s.m21);
	m1->s.m02 = (m1->s.m00 * m2->s.m02) + (m1->s.m01 * m2->s.m12) + (m1->s.m02 * m2->s.m22);

	m1->s.m10 = (m1->s.m10 * m2->s.m00) + (m1->s.m11 * m2->s.m10) + (m1->s.m12 * m2->s.m20);
	m1->s.m11 = (m1->s.m10 * m2->s.m01) + (m1->s.m11 * m2->s.m11) + (m1->s.m12 * m2->s.m21);
	m1->s.m12 = (m1->s.m10 * m2->s.m02) + (m1->s.m11 * m2->s.m12) + (m1->s.m12 * m2->s.m22);

	m1->s.m20 = (m1->s.m20 * m2->s.m00) + (m1->s.m21 * m2->s.m10) + (m1->s.m22 * m2->s.m20);
	m1->s.m21 = (m1->s.m20 * m2->s.m01) + (m1->s.m21 * m2->s.m11) + (m1->s.m22 * m2->s.m21);
	m1->s.m22 = (m1->s.m20 * m2->s.m02) + (m1->s.m21 * m2->s.m12) + (m1->s.m22 * m2->s.m22);
}

__inline static void Matrix4fSetZero(Matrix4f *mat)
{
	mat->s.m00 = mat->s.m01 = mat->s.m02 = mat->s.m03 = 0.0f;
	mat->s.m10 = mat->s.m11 = mat->s.m12 = mat->s.m13 = 0.0f;
	mat->s.m20 = mat->s.m21 = mat->s.m22 = mat->s.m23 = 0.0f;
	mat->s.m30 = mat->s.m31 = mat->s.m32 = mat->s.m33 = 0.0f;
}

__inline static void Matrix4fSetIdentity(Matrix4f * mat)
{
	Matrix4fSetZero(mat);

	mat->s.m00 = mat->s.m11 = mat->s.m22 = mat->s.m33 = 1.0f;
}

__inline static void Matrix4fSetRotationScaleFromMatrix4f(Matrix4f * m1, const Matrix4f * m2)
{
	AppAssert(m1 && m2);

	m1->s.xx = m2->s.xx; m1->s.yx = m2->s.yx; m1->s.zx = m2->s.zx;
	m1->s.xy = m2->s.xy; m1->s.yy = m2->s.yy; m1->s.zy = m2->s.zy;
	m1->s.xz = m2->s.xz; m1->s.yz = m2->s.yz; m1->s.zz = m2->s.zz;
}

__inline static float Matrix4fSVD(const Matrix4f *m, Matrix3f *rot3, Matrix4f *rot4)
{
	float s, n;

	AppAssert(m);

	// this is a simple svd.
	// Not complete but fast and reasonable.
	// See comment in Matrix3d.

	s = sqrtf(
	        ((m->s.xx * m->s.xx) + (m->s.xy * m->s.xy) + (m->s.xz * m->s.xz) +
	         (m->s.yx * m->s.yx) + (m->s.yy * m->s.yy) + (m->s.yz * m->s.yz) +
	         (m->s.zx * m->s.zx) + (m->s.zy * m->s.zy) + (m->s.zz * m->s.zz)) / 3.0f);

	if (rot3) {
		rot3->s.xx = m->s.xx; rot3->s.xy = m->s.xy; rot3->s.xz = m->s.xz;
		rot3->s.yx = m->s.yx; rot3->s.yy = m->s.yy; rot3->s.yz = m->s.yz;
		rot3->s.zx = m->s.zx; rot3->s.zy = m->s.zy; rot3->s.zz = m->s.zz;

		// zero-div may occur.

		n = 1.0f / sqrtf((m->s.xx * m->s.xx) + (m->s.xy * m->s.xy) + (m->s.xz * m->s.xz));
		rot3->s.xx *= n;
		rot3->s.xy *= n;
		rot3->s.xz *= n;

		n = 1.0f / sqrtf((m->s.yx * m->s.yx) + (m->s.yy * m->s.yy) + (m->s.yz * m->s.yz));
		rot3->s.yx *= n;
		rot3->s.yy *= n;
		rot3->s.yz *= n;

		n = 1.0f / sqrtf((m->s.zx * m->s.zx) + (m->s.zy * m->s.zy) + (m->s.zz * m->s.zz));
		rot3->s.zx *= n;
		rot3->s.zy *= n;
		rot3->s.zz *= n;
	}

	if (rot4) {
		if (rot4 != m)
			Matrix4fSetRotationScaleFromMatrix4f(rot4, m);

		// zero-div may occur.

		n = 1.0f / sqrtf((m->s.xx * m->s.xx) + (m->s.xy * m->s.xy) + (m->s.xz * m->s.xz));
		rot4->s.xx *= n;
		rot4->s.xy *= n;
		rot4->s.xz *= n;

		n = 1.0f / sqrtf((m->s.yx * m->s.yx) + (m->s.yy * m->s.yy) + (m->s.yz * m->s.yz));
		rot4->s.yx *= n;
		rot4->s.yy *= n;
		rot4->s.yz *= n;

		n = 1.0f / sqrtf((m->s.zx * m->s.zx) + (m->s.zy * m->s.zy) + (m->s.zz * m->s.zz));
		rot4->s.zx *= n;
		rot4->s.zy *= n;
		rot4->s.zz *= n;
	}

	return s;
}

__inline static void Matrix4fSetRotationScaleFromMatrix3f(Matrix4f *m1, const Matrix3f *m2)
{
	AppAssert(m1 && m2);

	m1->s.xx = m2->s.xx; m1->s.yx = m2->s.yx; m1->s.zx = m2->s.zx;
	m1->s.xy = m2->s.xy; m1->s.yy = m2->s.yy; m1->s.zy = m2->s.zy;
	m1->s.xz = m2->s.xz; m1->s.yz = m2->s.yz; m1->s.zz = m2->s.zz;
}

__inline static void Matrix4fMulRotationScale(Matrix4f *m, float scale)
{
	AppAssert(m);

	m->s.xx *= scale; m->s.yx *= scale; m->s.zx *= scale;
	m->s.xy *= scale; m->s.yy *= scale; m->s.zy *= scale;
	m->s.xz *= scale; m->s.yz *= scale; m->s.zz *= scale;
}

__inline static void Matrix4fSetRotationFromMatrix3f(Matrix4f *m1, const Matrix3f *m2)
{
	float scale;

	AppAssert(m1 && m2);

	scale = Matrix4fSVD(m1, 0, 0);

	Matrix4fSetRotationScaleFromMatrix3f(m1, m2);
	Matrix4fMulRotationScale(m1,scale);
}

//-------------------------------------------------------//

typedef struct {
	float x1,y1;
	float x2,y2;
} Line;

Line *line_new(void);
Line *line_newset(float x1, float y1, float x2, float y2);

void line_set(Line *l,float x1, float y1, float x2, float y2);

__inline void line_free(Line *l) { free(l); }

unsigned char line_intersect(const Line *l1,const Line *l2, float *x, float *y);

//-------------------------------------------------------//

typedef struct {
	int x,y;
	int w,h;
} URectangle; //Blame wingdi for this name

__inline void rectangle_set(URectangle *r,int x, int y, unsigned int w, unsigned int h)
{
	r->x = x;
	r->y = y;
	r->w = w;
	r->h = h;
}

__inline void rectangle_cpy(URectangle *r1,const URectangle *r2)
{
	r1->x = r2->x;
	r1->y = r2->y;
	r1->w = r2->w;
	r1->h = r2->h;
}

__inline unsigned char rectangle_intersect(const URectangle *r1, const URectangle *r2)
{
	return (r1->x+r1->w >= r2->x || r1->x <= r2->x+r2->w) &&
		   (r1->y+r1->h >= r2->y || r1->y <= r2->y+r2->h);
}

__inline unsigned char rectangle_contains(const URectangle *r1, const URectangle *r2)
{
	return (r1->x+r1->w >= r2->x+r2->w && r1->x <= r2->x) &&
		   (r1->y+r1->h >= r2->y+r2->h && r1->y <= r2->y);
}

__inline unsigned char rectangle_contains_point(const URectangle *r, int x, int y)
{
	return (r->x+r->w >= x && r->x <= x) &&
		   (r->y+r->h >= y && r->y <= y);
}

__inline unsigned char rectangle_equal(const URectangle *r1, const URectangle *r2)
{
	return r2->x == r1->x && r2->y == r1->y && r2->w == r1->w && r2->h == r1->h;
}

__inline void rectangle_move(URectangle *r, int x, int y)
{
	r->x = x;
	r->y = y;
}

__inline void rectangle_resize(URectangle *r, unsigned int w, unsigned int h)
{
	r->w = w;
	r->h = h;
}

__inline void rectangle_merge(URectangle *r1, const URectangle *r2)
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

//-------------------------------------------------------

typedef struct {
	int x,y,z;
	int w,h,d;
} Cuboid;

__inline void cuboid_cpy(Cuboid *c1,const Cuboid *c2)
{
	c1->x = c2->x;
	c1->y = c2->y;
	c1->z = c2->z;
	c1->w = c2->w;
	c1->h = c2->h;
	c1->d = c2->d;
}



//-------------------------------------------------------//

#endif
