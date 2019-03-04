#pragma once


#include <math.h>
#include "ogldev_math_3d.h"
#include "Camera.h"
// 具体角度值

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

	//Setup m_persProj
	void SetPerspectiveProj(float FOV = 30.0f, int WINDOW_WIDTH = 1024, int WINDOW_HEIGHT = 768, float zNear = 1.0f, float zFar = 1000.0f)
	{
		m_persProj.FOV = FOV; m_persProj.Height = WINDOW_HEIGHT; m_persProj.Width = WINDOW_WIDTH; m_persProj.zFar = zFar; m_persProj.zNear = zNear;
	}
	void SetPerspectiveProj(const PersProjInfo& p)
	{
		m_persProj = p;
	}

	// Set up Camera
	void SetCamera(const Camera& camera)
	{
		m_camera.Pos = camera.GetPos();
		m_camera.Target = camera.GetTarget();
		m_camera.Up = camera.GetUp();
	}

	const Matrix4f* GetTrans();

private:
	Vector3f m_scale;
	Vector3f m_worldPos;
	Vector3f m_rotateInfo;
	Matrix4f m_transformation;
	PersProjInfo m_persProj;
	struct
	{
		Vector3f Pos;
		Vector3f Target;
		Vector3f Up;
	} m_camera;
};

inline const Matrix4f* Pipeline::GetTrans()
{
	Matrix4f ScaleTrans, RotateTrans, TranslationTrans, PersProjTrans, CameraRotateTrans, CameraTranslationTrans;

	ScaleTrans.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
	RotateTrans.InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
	TranslationTrans.InitTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);
	PersProjTrans.InitPersProjTransform(m_persProj);
	CameraTranslationTrans.InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
	CameraRotateTrans.InitCameraTransform(m_camera.Target,m_camera.Up);
	m_transformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * TranslationTrans * RotateTrans * ScaleTrans;
	return &m_transformation;
}