/**
 * @file Camera.cpp
 * @brief
 * @author Yu Kimura
 * @date 2018/12/18
 */

#include "Camera.h"

#include "Buffer/CameraBuffer.h"

#include "Param/BufferParam.h"

#include "Input/Input.h"

namespace Simple 
{
    constexpr float Z_NEAR = 0.1f;
    constexpr float Z_FAR = 100000.0f;

    // コンストラクタ
    Camera::Camera(const UINT windowWidth, const UINT windowHeight) :
        pGraphicsResource(),
        m_eye(0.0f, 0.0f, -10.0f),
        m_forcus(0.0f, 0.0f, 0.0f),
        m_up(0.0f, 1.0f, 0.0f),
        m_speed(30.0f)
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

    void Camera::UpdateSpeed(const Input& input)
    {
        float ratio = input.GetMousePosZ();
        if (ratio < 0.1f && ratio > -0.1f)
            return;

        ratio = (ratio / std::fabsf(ratio));
        m_speed += ratio;

        const float minSpeed = 1.0f;
        const float maxSpeed = 100.0f;

        if (m_speed < minSpeed) 
            m_speed = minSpeed;
        else if (m_speed > maxSpeed) 
            m_speed = maxSpeed;
    }

    void Camera::Update(const Input& input)
    {
        UpdateSpeed(input);

        VECTOR3 forwardVector = GetCameraForwardVector();
        VECTOR3 rightVector = GetCameraRightVector();
        VECTOR3 upVector = GetCameraUpVector();

        if (input.IsPressKey(DIK_W)) 
        {
            m_eye += forwardVector * m_speed;
            m_forcus += forwardVector * m_speed;
        }
        if (input.IsPressKey(DIK_S)) 
        {
            m_eye -= forwardVector * m_speed;
            m_forcus -= forwardVector * m_speed;
        }
        if (input.IsPressKey(DIK_A)) 
        {
            m_eye -= rightVector * m_speed;
            m_forcus -= rightVector * m_speed;
        }
        if (input.IsPressKey(DIK_D)) 
        {
            m_eye += rightVector * m_speed;
            m_forcus += rightVector * m_speed;
        }

        if (input.IsTriggerLeftMouse()) 
        {
            m_forcus += rightVector * input.GetMousePosX();
            m_forcus += upVector * input.GetMousePosY();
        }
        if (input.IsTriggerRightMouse()) 
        {
            //float r = Vec3Lenght(m_forcus - m_eye);
            //
            //float theta = rotate.y + input.GetMousePosY() * speed;
            //float phi = rotate.x + input.GetMousePosX() * speed;
            //
            //VECTOR3 pos;
            //pos.x = r * sinf(theta) * cosf(phi);
            //pos.y = r * cosf(theta);
            //pos.z = r * sinf(theta) * sinf(phi);
            //
            //m_eye = m_forcus + pos;
            //
            //rotate.x = phi;
            //rotate.y = theta;
        }

        DirectX::XMVECTOR eye = { m_eye.x, m_eye.y, m_eye.z };
        DirectX::XMVECTOR forcus = { m_forcus.x, m_forcus.y, m_forcus.z };
        DirectX::XMVECTOR up = { m_up.x, m_up.y, m_up.z };

        m_matView.dx_m = DirectX::XMMatrixLookAtLH(eye, forcus, up);
    }

    void Camera::InitializeGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        BufferParam param;
        param.byteWidth = sizeof(CameraBuffer);
        param.byteWidthStride = sizeof(CameraBuffer);

        pGraphics->InitializeGraphicsBufferResource(pGraphicsResource, &param, Graphics::GraphicsResourceType::CBV);
    }

    void Camera::SetGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        BufferParam param;
        param.byteWidth = sizeof(CameraBuffer);
        param.byteWidthStride = sizeof(CameraBuffer);

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

        pGraphics->UpdateGraphicsBufferResource(pGraphicsResource, &cameraBuffer, &param, Graphics::GraphicsResourceType::CBV);

        pGraphics->SetConstantBufferResource(1, pGraphicsResource);
    }

} // namespace