#pragma once
#include "ogldev_math_3d.h"

class Camera
{
public:
	Camera(int WindowWidth, int WindowHeight);
	Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);
	bool OnKeyBoard(int Key);
	const Vector3f& GetPos() const
	{
		return m_pos;
	}

	const Vector3f& GetTarget() const
	{
		return m_target;
	}

	const Vector3f& GetUp() const
	{
		return m_up;
	}

	void Init();

	void OnMouse(int x, int y);

	void OnRender();

	void Update();

private:
	Vector3f m_pos;
	Vector3f m_target;
	Vector3f m_up;
	int m_windowWidth;
	int m_windowHeight;
	float m_AngleH;
	float m_AngleV;

	bool m_OnUpperEdge;
	bool m_OnLowerEdge;
	bool m_OnLeftEdge;
	bool m_OnRightEdge;

	Vector2f m_mousePos;
};