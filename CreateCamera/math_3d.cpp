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