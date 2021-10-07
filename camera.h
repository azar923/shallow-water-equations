#ifndef CAMERA_H
#define	CAMERA_H

#include "math_3d.h"


class Camera
{
public:

    Camera(int WindowWidth, int WindowHeight);

    Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

    bool OnKeyboard(int Key);

    void OnMouse(int x, int y);

    void OnRender();

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

	void SetPos( const float& x, const float& y, const float& z )
	{
		m_pos = Vector3f( x, y, z );
	}

	void SetTarget( const float& x, const float& y, const float& z )
	{
		m_target = Vector3f( x, y, z );
		m_target.Normalize();
	}

	void SetUp( const float& x, const float& y, const float& z )
	{
		m_up = Vector3f( x, y, z );
	}
	void SetCamera(int WindowWidth, int WindowHeight,const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);
	
private:

    void Update();

	void Init();



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

    Vector2i m_mousePos;
};

#endif	/* CAMERA_H */

