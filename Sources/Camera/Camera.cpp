/**
 * @file Camera.cpp
 * @brief
 * @author 木村優
 * @date 2018/12/18
 */

#include "Camera.h"
#include "CameraBufferType.h"
#include "Param/ConstantBufferParam.h"

namespace Simple 
{
    constexpr float Z_NEAR = 0.1f;
    constexpr float Z_FAR = 100000.0f;

    // コンストラクタ
    Camera::Camera(const UINT windowWidth, const UINT windowHeight) :
        m_pGraphicsResource(),
        m_eye(0.0f, 0.0f, -10.0f),
        m_forcus(),
        m_up(0.0f, 1.0f, 0.0f),
        m_speed(1.0f)
    {
        m_matProj.dx_m = DirectX::XMMatrixPerspectiveFovLH(
            DegreeToRadian(90.0f),
            (float)windowWidth / (float)windowHeight,
            Z_NEAR,
            Z_FAR);
    }

    // デストラクタ
    Camera::~Camera()
    {}

    // ビュー行列とプロジェクション行列を合成し取得
    Matrix Camera::GetViewProjectionMatrix() 
    {
        Matrix matVP;
        matVP.dx_m = m_matView.dx_m * m_matProj.dx_m;
        
        return matVP;
    }

    // ビュー逆行列を取得
    Matrix Camera::GetInvViewMatrix() 
    {
        Matrix matView;
        matView.dx_m = DirectX::XMMatrixInverse(nullptr, m_matView.dx_m);

        matView._41 = matView._42 = matView._43 = 0.0f;

        return matView;
    }

    // プロジェクション逆行列を取得
    Matrix Camera::GetInvProjectionMatrix() 
    {
        Matrix matProj;
        matProj.dx_m = DirectX::XMMatrixInverse(nullptr, m_matProj.dx_m);

        return matProj;
    }

    // カメラの右向きベクトルを取得
    VECTOR3 Camera::GetCameraRightVector() 
    {
        return VECTOR3(m_matView._11, m_matView._21, m_matView._31);
    }

    // カメラの上向きベクトルを取得
    VECTOR3 Camera::GetCameraUpVector() 
    {
        return VECTOR3(m_matView._12, m_matView._22, m_matView._32);
    }

    // カメラの前向きベクトルを取得
    VECTOR3 Camera::GetCameraForwardVector() 
    {
        return VECTOR3(m_matView._13, m_matView._23, m_matView._33);
    }

    void Camera::InitializeGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        ConstantBufferParam param;
        param.byteWidth = sizeof(CameraBuffer);

        pGraphics->InitializeGraphicsResource(m_pGraphicsResource, &param);

        Matrix matVP = GetViewProjectionMatrix();
        matVP.dx_m = DirectX::XMMatrixTranspose(matVP.dx_m);

        Matrix matView = m_matView;
        matView.dx_m = DirectX::XMMatrixTranspose(matView.dx_m);

        Matrix matProj = m_matProj;
        matProj.dx_m = DirectX::XMMatrixTranspose(matProj.dx_m);

        Matrix invMatProj = GetInvProjectionMatrix();

        CameraBuffer cameraBuffer = 
        {
            matVP,
            m_eye,
            0.0f,
            matView,
            matProj,
            invMatProj,
        };

        pGraphics->UpdateGraphicsResource(m_pGraphicsResource, &cameraBuffer);
    }

} // namespace