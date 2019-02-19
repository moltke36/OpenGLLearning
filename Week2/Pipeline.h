#pragma once


#include <math.h>
#include "ogldev_math_3d.h"
#define M_PI 3.14159265358979323846264338327950288
// 具体角度值
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

class Pipeline
{
public:
	// Constructor
	Pipeline()
	{
		m_scale = Vector3f(1.0f, 1.0f, 1.0f);
		m_worldPos = Vector3f(0.0f, 0.0f, 0.0f);
		m_rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);
	}

	// Destruct
	//~Pipeline()
	//{
	//	delete[] m_scale;
	//	delete[] m_worldPos;
	//	delete[] m_rotateInfo;
	//	delete[] m_transformation;
	//}

	//Setup m_persProj
	void SetPerspectiveProj(float FOV = 30.0f, int WINDOW_WIDTH = 1024, int WINDOW_HEIGHT = 768, float zNear = 1.0f, float zFar = 1000.0f) 
	{m_persProj.FOV = FOV; m_persProj.Height = WINDOW_HEIGHT; m_persProj.Width = WINDOW_WIDTH; m_persProj.zFar = zFar; m_persProj.zNear = zNear;}

	// Scale Function
	void Scale(float ScaleX, float ScaleY, float ScaleZ) { m_scale.x = ScaleX; m_scale.y = ScaleY; m_scale.z = ScaleZ; }
	void Scale(float s) { Scale(s,s,s); }
	void Scale(const Vector3f & scale) { Scale(scale.x, scale.y, scale.z); }

	// WorldPos Function
	void WorldPos(float x, float y, float z) {m_worldPos.x = x; m_worldPos.y = y; m_worldPos.z = z;}
	void WorldPos(const Vector3f &pos) {WorldPos(pos.x, pos.y, pos.z);};

	// Rotate Function
	void Rotate(float RotateX, float RotateY, float RotateZ) {m_rotateInfo.x = RotateX; m_rotateInfo.y = RotateY; m_rotateInfo.z = RotateZ;}
	void Rotate(const Vector3f &Rot) { Rotate(Rot.x, Rot.y, Rot.z); }

	const Matrix4f* GetTrans();

private:
	Vector3f m_scale;
	Vector3f m_worldPos;
	Vector3f m_rotateInfo;
	Matrix4f m_transformation;
	PersProjInfo m_persProj;

	//void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
	const void InitScaleTransform(Matrix4f& ScaleMat4);
	//void InitRotateTransform(const Quaternion& quat);
	const void InitRotateTransform(Matrix4f& RotMat4);
	const void InitPerspectiveProj(Matrix4f & m) const;
	//void InitTranslationTransform(float x, float y, float z);
	const void InitTranslationTransform(Matrix4f& TransMat4);

};

inline const Matrix4f* Pipeline::GetTrans()
{
	Matrix4f ScaleTrans, RotateTrans, TranslationTrans, PersProjTrans;
	InitScaleTransform(ScaleTrans);
	InitRotateTransform(RotateTrans);
	InitTranslationTransform(TranslationTrans);
	InitPerspectiveProj(PersProjTrans);
	m_transformation = m_transformation* PersProjTrans *TranslationTrans * RotateTrans * ScaleTrans;
	return &m_transformation;

	
}

inline const void Pipeline::InitTranslationTransform(Matrix4f& TransMat4)
{
	// Translation
	TransMat4.m[0][0] = 1.0f; TransMat4.m[0][1] = 0.0f; TransMat4.m[0][2] = 0.0f; TransMat4.m[0][3] = m_worldPos.x;
	TransMat4.m[1][0] = 0.0f; TransMat4.m[1][1] = 1.0f; TransMat4.m[1][2] = 0.0f; TransMat4.m[1][3] = m_worldPos.y;
	TransMat4.m[2][0] = 0.0f; TransMat4.m[2][1] = 0.0f; TransMat4.m[2][2] = 1.0f	; TransMat4.m[2][3] = m_worldPos.z;
	TransMat4.m[3][0] = 0.0f; TransMat4.m[3][1] = 0.0f; TransMat4.m[3][2] = 0.0f; TransMat4.m[3][3] = 1.0f;
} 

inline const void Pipeline::InitScaleTransform(Matrix4f& ScaleMat4)
{
	// Translation
	ScaleMat4.m[0][0] = m_scale.x; ScaleMat4.m[0][1] = 0.0f; ScaleMat4.m[0][2] = 0.0f;		ScaleMat4.m[0][3] = 0.0;
	ScaleMat4.m[1][0] = 0.0f; ScaleMat4.m[1][1] = m_scale.y; ScaleMat4.m[1][2] = 0.0f;		ScaleMat4.m[1][3] = 0.0;
	ScaleMat4.m[2][0] = 0.0f; ScaleMat4.m[2][1] = 0.0f;		 ScaleMat4.m[2][2] = m_scale.z; ScaleMat4.m[2][3] = 0.0;
	ScaleMat4.m[3][0] = 0.0f; ScaleMat4.m[3][1] = 0.0f;		 ScaleMat4.m[3][2] = 0.0f;		ScaleMat4.m[3][3] = 1.0f;
}

inline const void Pipeline::InitRotateTransform(Matrix4f& RotMat4)
{
	Matrix4f RotateX, RotateY, RotateZ;
	// Translation
	RotateX.m[0][0] = 1.0f;		 RotateX.m[0][1] = 0.0f;							RotateX.m[0][2] = 0.0f;								RotateX.m[0][3] = 0.0f;
	RotateX.m[1][0] = 0.0f;		 RotateX.m[1][1] = cosf(ToRadian(m_rotateInfo.x));	RotateX.m[1][2] = -sinf(ToRadian(m_rotateInfo.x));	RotateX.m[1][3] = 0.0f;
	RotateX.m[2][0] = 0.0f;		 RotateX.m[2][1] = sinf(ToRadian(m_rotateInfo.x));  RotateX.m[2][2] = cosf(ToRadian(m_rotateInfo.x));	RotateX.m[2][3] = 0.0f;
	RotateX.m[3][0] = 0.0f;      RotateX.m[3][1] = 0.0f;							RotateX.m[3][2] = 0.0f;								RotateX.m[3][3] = 1.0f;

	RotateY.m[0][0] = cosf(ToRadian(m_rotateInfo.y));	 RotateY.m[0][1] = 0.0f;		RotateY.m[0][2] = -sinf(ToRadian(m_rotateInfo.y));	RotateY.m[0][3] = 0.0f;
	RotateY.m[1][0] = 0.0f;								 RotateY.m[1][1] = 1.0f;		RotateY.m[1][2] = 0.0f;								RotateY.m[1][3] = 0.0f;
	RotateY.m[2][0] = sinf(ToRadian(m_rotateInfo.y));	 RotateY.m[2][1] = 0.0f;		RotateY.m[2][2] = cosf(ToRadian(m_rotateInfo.y));	RotateY.m[2][3] = 0.0f;
	RotateY.m[3][0] = 0.0f;								 RotateY.m[3][1] = 0.0f;		RotateY.m[3][2] = 0.0f;								RotateY.m[3][3] = 1.0f;

	RotateZ.m[0][0] = cosf(ToRadian(m_rotateInfo.z));	 RotateZ.m[0][1] = -sinf(ToRadian(m_rotateInfo.z));	RotateZ.m[0][2] = 0.0f;		RotateZ.m[0][3] = 0.0f;
	RotateZ.m[1][0] = sinf(ToRadian(m_rotateInfo.z));	 RotateZ.m[1][1] = cosf(ToRadian(m_rotateInfo.z));	RotateZ.m[1][2] = 0.0f;		RotateZ.m[1][3] = 0.0f;
	RotateZ.m[2][0] = 0.0f;								 RotateZ.m[2][1] = 0.0f;							RotateZ.m[2][2] = 1.0f;		RotateZ.m[2][3] = 0.0f;
	RotateZ.m[3][0] = 0.0f;								 RotateZ.m[3][1] = 0.0f;							RotateZ.m[3][2] = 0.0f;		RotateZ.m[3][3] = 1.0f;

	RotMat4 = RotMat4 * RotateX*RotateY*RotateZ;
}

inline const void Pipeline::InitPerspectiveProj(Matrix4f& m) const
{
	const float ar = m_persProj.Width / m_persProj.Height;
	const float zNear = m_persProj.zNear;
	const float zFar = m_persProj.zFar;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(ToRadian(m_persProj.FOV / 2.0));

	m.m[0][0] = 1.0f / (tanHalfFOV * ar);
	m.m[0][1] = 0.0f;
	m.m[0][2] = 0.0f;
	m.m[0][3] = 0.0f;

	m.m[1][0] = 0.0f;
	m.m[1][1] = 1.0f / tanHalfFOV;
	m.m[1][2] = 0.0f;
	m.m[1][3] = 0.0f;

	m.m[2][0] = 0.0f;
	m.m[2][1] = 0.0f;
	m.m[2][2] = (-zNear - zFar) / zRange;
	m.m[2][3] = 2.0f * zFar * zNear / zRange;

	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = 1.0f;
	m.m[3][3] = 0.0f;
}