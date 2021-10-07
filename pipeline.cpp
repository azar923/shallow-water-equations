
#include "pipeline.h"


const Matrix4f* Pipeline::GetWVPTrans()
{
    Matrix4f ScaleTrans, RotateTrans, TranslationTrans, CameraTranslationTrans, CameraRotateTrans, PersProjTrans;

    ScaleTrans.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
    RotateTrans.InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
    TranslationTrans.InitTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);
    CameraTranslationTrans.InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
    CameraRotateTrans.InitCameraTransform(m_camera.Target, m_camera.Up);
    PersProjTrans.InitPersProjTransform(m_persProj.FOV, m_persProj.Width, m_persProj.Height, m_persProj.zNear, m_persProj.zFar);

    m_WVP_transformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * TranslationTrans * RotateTrans * ScaleTrans;
    return &m_WVP_transformation;
}

const Matrix4f* Pipeline::GetWVP_Ortho_Trans()
{
    Matrix4f OrthoTrans;

	OrthoTrans.InitOrthoProjTransform(m_orthoProj.left, m_orthoProj.right, m_orthoProj.top, m_orthoProj.bottom, m_orthoProj.zNear, m_orthoProj.zFar);

	m_WVP_transformation = OrthoTrans;
    return &m_WVP_Ortho_transformation;
}


const Matrix4f* Pipeline::GetWorldTrans()
{
    Matrix4f ScaleTrans, RotateTrans, TranslationTrans;

    ScaleTrans.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
    RotateTrans.InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
    TranslationTrans.InitTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);
   
    m_World_transformation = TranslationTrans * RotateTrans * ScaleTrans;
    return &m_World_transformation;
}

const Matrix4f* Pipeline::GetVPTrans()
{
    Matrix4f CameraTranslationTrans, CameraRotateTrans, PersProjTrans;

    CameraTranslationTrans.InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
    CameraRotateTrans.InitCameraTransform(m_camera.Target, m_camera.Up);
    PersProjTrans.InitPersProjTransform(m_persProj.FOV, m_persProj.Width, m_persProj.Height, m_persProj.zNear, m_persProj.zFar);
   
     m_VP_transformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans;
    return &m_VP_transformation;
}


