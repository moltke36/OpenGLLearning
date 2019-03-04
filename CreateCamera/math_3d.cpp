#include "stdafx.h"
#include <stdlib.h>
#include "ogldev_math_3d.h"

void Matrix4f::InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ)
{
	// Translation
	m[0][0] = ScaleX; m[0][1] = 0.0f; m[0][2] = 0.0f;		m[0][3] = 0.0;
	m[1][0] = 0.0f; m[1][1] = ScaleY; m[1][2] = 0.0f;		m[1][3] = 0.0;
	m[2][0] = 0.0f; m[2][1] = 0.0f;		 m[2][2] = ScaleZ; m[2][3] = 0.0;
	m[3][0] = 0.0f; m[3][1] = 0.0f;		 m[3][2] = 0.0f;		m[3][3] = 1.0f;
}

void Matrix4f::InitTranslationTransform(float x, float y, float z)
{
	// Translation
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void Matrix4f::InitRotateTransform(float RotateX, float RotateY, float RotateZ)
{
	Matrix4f RotateXm, RotateYm, RotateZm;
	// Translation
	RotateXm.m[0][0] = 1.0f;		 RotateXm.m[0][1] = 0.0f;							RotateXm.m[0][2] = 0.0f;								RotateXm.m[0][3] = 0.0f;
	RotateXm.m[1][0] = 0.0f;		 RotateXm.m[1][1] = cosf(ToRadian(RotateX));	RotateXm.m[1][2] = -sinf(ToRadian(RotateX));	RotateXm.m[1][3] = 0.0f;
	RotateXm.m[2][0] = 0.0f;		 RotateXm.m[2][1] = sinf(ToRadian(RotateX));  RotateXm.m[2][2] = cosf(ToRadian(RotateX));	RotateXm.m[2][3] = 0.0f;
	RotateXm.m[3][0] = 0.0f;      RotateXm.m[3][1] = 0.0f;							RotateXm.m[3][2] = 0.0f;								RotateXm.m[3][3] = 1.0f;

	RotateYm.m[0][0] = cosf(ToRadian(RotateY));	 RotateYm.m[0][1] = 0.0f;		RotateYm.m[0][2] = -sinf(ToRadian(RotateY));	RotateYm.m[0][3] = 0.0f;
	RotateYm.m[1][0] = 0.0f;								 RotateYm.m[1][1] = 1.0f;		RotateYm.m[1][2] = 0.0f;								RotateYm.m[1][3] = 0.0f;
	RotateYm.m[2][0] = sinf(ToRadian(RotateY));	 RotateYm.m[2][1] = 0.0f;		RotateYm.m[2][2] = cosf(ToRadian(RotateY));	RotateYm.m[2][3] = 0.0f;
	RotateYm.m[3][0] = 0.0f;								 RotateYm.m[3][1] = 0.0f;		RotateYm.m[3][2] = 0.0f;								RotateYm.m[3][3] = 1.0f;

	RotateZm.m[0][0] = cosf(ToRadian(RotateZ));	 RotateZm.m[0][1] = -sinf(ToRadian(RotateZ));	RotateZm.m[0][2] = 0.0f;		RotateZm.m[0][3] = 0.0f;
	RotateZm.m[1][0] = sinf(ToRadian(RotateZ));	 RotateZm.m[1][1] = cosf(ToRadian(RotateZ));	RotateZm.m[1][2] = 0.0f;		RotateZm.m[1][3] = 0.0f;
	RotateZm.m[2][0] = 0.0f;								 RotateZm.m[2][1] = 0.0f;							RotateZm.m[2][2] = 1.0f;		RotateZm.m[2][3] = 0.0f;
	RotateZm.m[3][0] = 0.0f;								 RotateZm.m[3][1] = 0.0f;							RotateZm.m[3][2] = 0.0f;		RotateZm.m[3][3] = 1.0f;

	*this = RotateXm * RotateYm * RotateZm;
}

void Matrix4f::InitCameraTransform(const Vector3f& Target, const Vector3f& Up)
{
	Vector3f N = Target;
	N.Normalize();
	Vector3f U = Up;
	U = U.Cross(Target);
	U.Normalize();
	Vector3f V = N.Cross(U);

	m[0][0] = U.x; m[0][1] = U.y; m[0][2] = U.z; m[0][3] = 0.0f;
	m[1][0] = V.x; m[1][1] = V.y; m[1][2] = V.z; m[1][3] = 0.0f;
	m[2][0] = N.x; m[2][1] = N.y; m[2][2] = N.z; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void Matrix4f::InitPersProjTransform(const PersProjInfo& p)
{
	const float ar = p.Width / p.Height;
	const float zNear = p.zNear;
	const float zFar = p.zFar;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(ToRadian(p.FOV / 2.0));

	m[0][0] = 1.0f / (tanHalfFOV * ar);
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = 1.0f / tanHalfFOV;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = (-zNear - zFar) / zRange;
	m[2][3] = 2.0f * zFar * zNear / zRange;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 1.0f;
	m[3][3] = 0.0f;
}

void Vector3f::Rotate(float Angle, const Vector3f& Axe)
{
	const float SinHalfAngle = sinf(ToRadian(Angle / 2));
	const float CosHalfAngle = cosf(ToRadian(Angle / 2));

	const float Rx = Axe.x * SinHalfAngle;
	const float Ry = Axe.y * SinHalfAngle;
	const float Rz = Axe.z * SinHalfAngle;
	const float Rw = CosHalfAngle;
	Quaternion RotationQ(Rx, Ry, Rz, Rw);

	Quaternion ConjugateQ = RotationQ.Conjugate();
	//  ConjugateQ.Normalize();
	Quaternion W = RotationQ * (*this) * ConjugateQ;

	x = W.x;
	y = W.y;
	z = W.z;
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

void Quaternion::Normalize()
{
	float Length = sqrtf(x * x + y * y + z * z + w * w);

	x /= Length;
	y /= Length;
	z /= Length;
	w /= Length;
}


Quaternion Quaternion::Conjugate()
{
	Quaternion ret(-x, -y, -z, w);
	return ret;
}

Quaternion operator*(const Quaternion& l, const Quaternion& r)
{
	const float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
	const float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
	const float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
	const float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

	Quaternion ret(x, y, z, w);

	return ret;
}

Quaternion operator*(const Quaternion& q, const Vector3f& v)
{
	const float w = -(q.x * v.x) - (q.y * v.y) - (q.z * v.z);
	const float x = (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
	const float y = (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
	const float z = (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

	Quaternion ret(x, y, z, w);

	return ret;
}


Vector3f Quaternion::ToDegrees()
{
	float f[3];

	f[0] = atan2(x * z + y * w, x * w - y * z);
	f[1] = acos(-x * x - y * y - z * z - w * w);
	f[2] = atan2(x * z - y * w, x * w + y * z);

	f[0] = ToDegree(f[0]);
	f[1] = ToDegree(f[1]);
	f[2] = ToDegree(f[2]);

	return Vector3f(f);
}
