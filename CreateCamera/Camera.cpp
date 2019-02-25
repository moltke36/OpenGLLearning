#include "stdafx.h"
#include "Camera.h"
#include <GL/freeglut.h>  //freeGLUT´°¿Ú¿â

const static float STEP_SCALE = 0.1f;
const static float EDGE_STEP = 0.5f;

Camera::Camera()
{
	m_pos = Vector3f(0.0f, 0.0f, 0.0f);
	m_target = Vector3f(0.0f, 0.0f, 1.0f);
	m_target.Normalize();
	m_up = Vector3f(0.0f, 1.0f, 0.0f);
}

Camera::Camera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
{

	m_pos = Pos;

	m_target = Target;
	m_target.Normalize();

	m_up = Up;
	m_up.Normalize();
}

bool Camera::OnKeyBoard(int Key)
{
	bool Ret = false;

	switch (Key) {

	case GLUT_KEY_UP:
	{
		m_pos += (m_target * STEP_SCALE);
		Ret = true;
	}
	break;

	case GLUT_KEY_DOWN:
	{
		m_pos -= (m_target * STEP_SCALE);
		Ret = true;
	}
	break;

	case GLUT_KEY_LEFT:
	{
		Vector3f Left = m_target.Cross(m_up);
		Left.Normalize();
		Left *= STEP_SCALE;
		m_pos += Left;
		Ret = true;
	}
	break;

	case GLUT_KEY_RIGHT:
	{
		Vector3f Right = m_up.Cross(m_target);
		Right.Normalize();
		Right *= STEP_SCALE;
		m_pos += Right;
		Ret = true;
	}
	break;
	}

	return Ret;
}