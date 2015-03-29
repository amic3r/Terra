
#ifndef _included_terra_matrix_h
#define _included_terra_matrix_h

// Common matrices

typedef union _Matrix3f{
	struct {
		union {float m00; float xx;}; union {float m10; float yx;}; union {float m20; float zx;};
		union {float m01; float xy;}; union {float m11; float yy;}; union {float m21; float zy;};
		union {float m02; float xz;}; union {float m12; float yz;}; union {float m22; float zz;};
	} s;
	float M[9];
} Matrix3f;

typedef union _Matrix4f {
	struct {
		union {float m00; float xx;}; union {float m10; float yx;}; union {float m20; float zx;}; union {float m30; float tx;};
		union {float m01; float xy;}; union {float m11; float yy;}; union {float m21; float zy;}; union {float m31; float ty;};
		union {float m02; float xz;}; union {float m12; float yz;}; union {float m22; float zz;}; union {float m32; float tz;};
		union {float m03; float xt;}; union {float m13; float yt;}; union {float m23; float zt;}; union {float m33; float tt;};
	} s;
	float M[16];
} Matrix4f;

// Matrix3f operations

inline static void Matrix3fSetZero(Matrix3f *matrix)
{
	memset(matrix,0, sizeof(matrix->M));
}

inline static void Matrix3fSetIdentity(Matrix3f *matrix)
{
	Matrix3fSetZero(matrix);
	matrix->s.m00 = matrix->s.m11 = matrix->s.m22 = 1.0f;
}

inline static void Matrix3fMulMatrix3f(Matrix3f *m1, const Matrix3f * m2)
{
	Matrix3f res;

	res.s.m00 = (m1->s.m00 * m2->s.m00) + (m1->s.m01 * m2->s.m10) + (m1->s.m02 * m2->s.m20);
	res.s.m01 = (m1->s.m00 * m2->s.m01) + (m1->s.m01 * m2->s.m11) + (m1->s.m02 * m2->s.m21);
	res.s.m02 = (m1->s.m00 * m2->s.m02) + (m1->s.m01 * m2->s.m12) + (m1->s.m02 * m2->s.m22);

	res.s.m10 = (m1->s.m10 * m2->s.m00) + (m1->s.m11 * m2->s.m10) + (m1->s.m12 * m2->s.m20);
	res.s.m11 = (m1->s.m10 * m2->s.m01) + (m1->s.m11 * m2->s.m11) + (m1->s.m12 * m2->s.m21);
	res.s.m12 = (m1->s.m10 * m2->s.m02) + (m1->s.m11 * m2->s.m12) + (m1->s.m12 * m2->s.m22);

	res.s.m20 = (m1->s.m20 * m2->s.m00) + (m1->s.m21 * m2->s.m10) + (m1->s.m22 * m2->s.m20);
	res.s.m21 = (m1->s.m20 * m2->s.m01) + (m1->s.m21 * m2->s.m11) + (m1->s.m22 * m2->s.m21);
	res.s.m22 = (m1->s.m20 * m2->s.m02) + (m1->s.m21 * m2->s.m12) + (m1->s.m22 * m2->s.m22);

	memcpy(m1,&res,sizeof(res));
}

inline static void Matrix3fSetScale(Matrix3f *matrix, float x, float y, float z)
{
	Matrix3f mat;
	Matrix3fSetZero(&mat);

	mat.s.xx = x;
	mat.s.yy = y;
	mat.s.zz = z;

	Matrix3fMulMatrix3f(matrix,&mat);
}

inline static void Matrix3fSetRotate(Matrix3f *matrix, float angle, float x, float y, float z)
{
	Matrix3f mat;
	float c = cosf(angle), oc = 1 - c;
	float s = sinf(angle);
	Vector v = {x,y,z};
	VectorNormalize(&v);
	x = v.x * v.x;
	y = v.y * v.y;
	z = v.z * v.z;

	mat.s.xx = c + (x * oc);
	mat.s.xy = (v.x * v.y * oc) - (v.z * s);
	mat.s.xz = (v.x * v.z * oc) + (v.y * s);

	mat.s.yx = (v.x * v.y * oc) + (v.z * s);
	mat.s.yy = c + (y * oc);
	mat.s.yz = (v.y * v.z * oc) - (v.x * s);

	mat.s.zx = (v.x * v.z * oc) - (v.y * s);
	mat.s.zy = (v.y * v.z * oc) + (v.x * s);
	mat.s.zz = c + (z * oc);

	Matrix3fMulMatrix3f(matrix,&mat);
}

inline static void Matrix3fSetRotationFromQuat(Matrix3f *matrix, const Quat * q1)
{
	float n, s;
	float xs, ys, zs;
	float wx, wy, wz;
	float xx, xy, xz;
	float yy, yz, zz;

	n = (q1->x * q1->x) + (q1->y * q1->y) + (q1->z * q1->z) + (q1->t * q1->t);
	s = (n > 0.0f) ? (2.0f / n) : 0.0f;

	xs = q1->x * s;  ys = q1->y * s;  zs = q1->z * s;
	wx = q1->t * xs; wy = q1->t * ys; wz = q1->t * zs;
	xx = q1->x * xs; xy = q1->x * ys; xz = q1->x * zs;
	yy = q1->y * ys; yz = q1->y * zs; zz = q1->z * zs;

	matrix->s.xx = 1.0f - (yy + zz); matrix->s.yx = xy - wz;          matrix->s.zx = xz + wy;
	matrix->s.xy = xy + wz;          matrix->s.yy = 1.0f - (xx + zz); matrix->s.zy = yz - wx;
	matrix->s.zx = xz - wy;          matrix->s.yz = yz + wx;          matrix->s.zz = 1.0f - (xx + yy);
}

// Matrix4f operations

inline static void Matrix4fSetZero(Matrix4f *matrix)
{
	memset(matrix,0,sizeof(matrix->M));
}

inline static void Matrix4fSetIdentity(Matrix4f *matrix)
{
	Matrix4fSetZero(matrix);

	matrix->s.m00 = matrix->s.m11 = matrix->s.m22 = matrix->s.m33 = 1.0f;
}

inline static void Matrix4fMulMatrix4f(Matrix4f *m1, const Matrix4f * m2)
{
	Matrix4f res;
	res.s.m00 = (m1->s.m00 * m2->s.m00) + (m1->s.m01 * m2->s.m10) + (m1->s.m02 * m2->s.m20) + (m1->s.m03 * m2->s.m30);
	res.s.m01 = (m1->s.m00 * m2->s.m01) + (m1->s.m01 * m2->s.m11) + (m1->s.m02 * m2->s.m21) + (m1->s.m03 * m2->s.m31);
	res.s.m02 = (m1->s.m00 * m2->s.m02) + (m1->s.m01 * m2->s.m12) + (m1->s.m02 * m2->s.m22) + (m1->s.m03 * m2->s.m32);
	res.s.m03 = (m1->s.m00 * m2->s.m03) + (m1->s.m01 * m2->s.m13) + (m1->s.m02 * m2->s.m23) + (m1->s.m03 * m2->s.m33);

	res.s.m10 = (m1->s.m10 * m2->s.m00) + (m1->s.m11 * m2->s.m10) + (m1->s.m12 * m2->s.m20) + (m1->s.m13 * m2->s.m30);
	res.s.m11 = (m1->s.m10 * m2->s.m01) + (m1->s.m11 * m2->s.m11) + (m1->s.m12 * m2->s.m21) + (m1->s.m13 * m2->s.m31);
	res.s.m12 = (m1->s.m10 * m2->s.m02) + (m1->s.m11 * m2->s.m12) + (m1->s.m12 * m2->s.m22) + (m1->s.m13 * m2->s.m32);
	res.s.m13 = (m1->s.m10 * m2->s.m03) + (m1->s.m11 * m2->s.m13) + (m1->s.m12 * m2->s.m23) + (m1->s.m13 * m2->s.m33);

	res.s.m20 = (m1->s.m20 * m2->s.m00) + (m1->s.m21 * m2->s.m10) + (m1->s.m22 * m2->s.m20) + (m1->s.m23 * m2->s.m30);
	res.s.m21 = (m1->s.m20 * m2->s.m01) + (m1->s.m21 * m2->s.m11) + (m1->s.m22 * m2->s.m21) + (m1->s.m23 * m2->s.m31);
	res.s.m22 = (m1->s.m20 * m2->s.m02) + (m1->s.m21 * m2->s.m12) + (m1->s.m22 * m2->s.m22) + (m1->s.m23 * m2->s.m32);
	res.s.m23 = (m1->s.m20 * m2->s.m03) + (m1->s.m21 * m2->s.m13) + (m1->s.m22 * m2->s.m23) + (m1->s.m23 * m2->s.m33);

	res.s.m30 = (m1->s.m30 * m2->s.m00) + (m1->s.m31 * m2->s.m10) + (m1->s.m32 * m2->s.m20) + (m1->s.m33 * m2->s.m30);
	res.s.m31 = (m1->s.m30 * m2->s.m01) + (m1->s.m31 * m2->s.m11) + (m1->s.m32 * m2->s.m21) + (m1->s.m33 * m2->s.m31);
	res.s.m32 = (m1->s.m30 * m2->s.m02) + (m1->s.m31 * m2->s.m12) + (m1->s.m32 * m2->s.m22) + (m1->s.m33 * m2->s.m32);
	res.s.m33 = (m1->s.m30 * m2->s.m03) + (m1->s.m31 * m2->s.m13) + (m1->s.m32 * m2->s.m23) + (m1->s.m33 * m2->s.m33);

	memcpy(m1,&res,sizeof(res));
}

inline static void Matrix4fSetTranslation(Matrix4f *matrix, float x, float y, float z)
{
	Matrix4f mat;

	Matrix4fSetIdentity(&mat);

	mat.s.xt = x;
	mat.s.yt = y;
	mat.s.zt = z;

	Matrix4fMulMatrix4f(matrix,&mat);
}

inline static void Matrix4fSetScale(Matrix4f *matrix, float x, float y, float z)
{
	Matrix4f mat;
	Matrix4fSetZero(&mat);

	mat.s.xx = x;
	mat.s.yy = y;
	mat.s.zz = z;
	mat.s.tt = 1.0f;

	Matrix4fMulMatrix4f(matrix,&mat);
}

inline static void Matrix4fSetRotation(Matrix4f *matrix, float angle, float x, float y, float z)
{
	Matrix4f mat;
	float c = cosf(angle), oc = 1 - c;
	float s = sinf(angle);
	Vector v = {x,y,z};
	VectorNormalize(&v);
	x = v.x * v.x;
	y = v.y * v.y;
	z = v.z * v.z;

	Matrix4fSetZero(&mat);

	mat.s.xx = c + (x * oc);
	mat.s.xy = (v.x * v.y * oc) - (v.z * s);
	mat.s.xz = (v.x * v.z * oc) + (v.y * s);

	mat.s.yx = (v.x * v.y * oc) + (v.z * s);
	mat.s.yy = c + (y * oc);
	mat.s.yz = (v.y * v.z * oc) - (v.x * s);

	mat.s.zx = (v.x * v.z * oc) - (v.y * s);
	mat.s.zy = (v.y * v.z * oc) + (v.x * s);
	mat.s.zz = c + (z * oc);

	mat.s.tt = 1.0f;

	Matrix4fMulMatrix4f(matrix,&mat);
}

inline static void Matrix4fSetRotationScaleFromMatrix4f(Matrix4f * m1, const Matrix4f * m2)
{
	m1->s.xx = m2->s.xx; m1->s.yx = m2->s.yx; m1->s.zx = m2->s.zx;
	m1->s.xy = m2->s.xy; m1->s.yy = m2->s.yy; m1->s.zy = m2->s.zy;
	m1->s.xz = m2->s.xz; m1->s.yz = m2->s.yz; m1->s.zz = m2->s.zz;
}

inline static float Matrix4fSVD(const Matrix4f *m, Matrix3f *rot3, Matrix4f *rot4)
{
	float s, n;

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

inline static void Matrix4fSetRotationScaleFromMatrix3f(Matrix4f *m1, const Matrix3f *m2)
{
	m1->s.xx = m2->s.xx; m1->s.yx = m2->s.yx; m1->s.zx = m2->s.zx;
	m1->s.xy = m2->s.xy; m1->s.yy = m2->s.yy; m1->s.zy = m2->s.zy;
	m1->s.xz = m2->s.xz; m1->s.yz = m2->s.yz; m1->s.zz = m2->s.zz;
}

inline static void Matrix4fMulRotationScale(Matrix4f *m, float scale)
{
	m->s.xx *= scale; m->s.yx *= scale; m->s.zx *= scale;
	m->s.xy *= scale; m->s.yy *= scale; m->s.zy *= scale;
	m->s.xz *= scale; m->s.yz *= scale; m->s.zz *= scale;
}

inline static void Matrix4fSetRotationFromMatrix3f(Matrix4f *m1, const Matrix3f *m2)
{
	float scale;

	scale = Matrix4fSVD(m1, 0, 0);

	Matrix4fSetRotationScaleFromMatrix3f(m1, m2);
	Matrix4fMulRotationScale(m1,scale);
}

#endif
