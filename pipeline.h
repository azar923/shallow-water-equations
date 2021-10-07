#ifndef PIPELINE_H
#define	PIPELINE_H

#include "math_3d.h"

class Pipeline
{
public:
    Pipeline()
    {
        m_scale      = Vector3f(1.0f, 1.0f, 1.0f);
        m_worldPos   = Vector3f(0.0f, 0.0f, 0.0f);
        m_rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);
    }

    void Scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        m_scale.x = ScaleX;
        m_scale.y = ScaleY;
        m_scale.z = ScaleZ;
    }

    void WorldPos(float x, float y, float z)
    {
        m_worldPos.x = x;
        m_worldPos.y = y;
        m_worldPos.z = z;
    }

    void Rotate(float RotateX, float RotateY, float RotateZ)
    {
        m_rotateInfo.x = RotateX;
        m_rotateInfo.y = RotateY;
        m_rotateInfo.z = RotateZ;
    }

    void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar)
    {
        m_persProj.FOV    = FOV;
        m_persProj.Width  = Width;
        m_persProj.Height = Height;
        m_persProj.zNear  = zNear;
        m_persProj.zFar   = zFar;
    }

	void SetOrthoProj(float left,float right, float bottom, float top, float zNear, float zFar)
	{
		m_orthoProj.left = left;
		m_orthoProj.right = right;
		m_orthoProj.bottom = bottom;
		m_orthoProj.top = top;
		m_orthoProj.zNear = zNear;
		m_orthoProj.zFar = zFar;
	}

    void SetCamera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
    {
        m_camera.Pos = Pos;
        m_camera.Target = Target;
        m_camera.Up = Up;
    }

    const Matrix4f* GetWVPTrans();
	const Matrix4f* GetWorldTrans();
	const Matrix4f* GetVPTrans(); 
	const Matrix4f* GetWVP_Ortho_Trans();

private:
    Vector3f m_scale;
    Vector3f m_worldPos;
    Vector3f m_rotateInfo;

    struct {
        float FOV;
        float Width;
        float Height;
        float zNear;
        float zFar;
    } m_persProj;

	struct {
		float left;
		float right;
		float bottom;
		float top;
		float zNear;
		float zFar;
	} m_orthoProj;

    struct {
        Vector3f Pos;
        Vector3f Target;
        Vector3f Up;
    } m_camera;

    Matrix4f m_WVP_transformation;
	Matrix4f m_World_transformation;
	Matrix4f m_VP_transformation;
	Matrix4f m_WVP_Ortho_transformation;
};


#endif	/* PIPELINE_H */

