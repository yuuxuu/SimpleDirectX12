/**
 * @file Camera.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/19
 */

#pragma once

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Math/math.h"

#include "GraphicsAPI/DirectX12/IGraphics.h"
#include "GraphicsAPI/DirectX12/DX12Resource/IDX12Resource.h"

namespace Simple 
{
    class Input;

    class Camera 
    {
    private:
        VECTOR3                         m_eye;
        VECTOR3                         m_forcus;
        VECTOR3                         m_up;

        Matrix                          m_matView;
        Matrix                          m_matProj;

        float                           m_speed;

        Graphics::IDX12Resouce*         pGraphicsResource;

    private:
        Camera(const Camera&) = delete;
        Camera operator=(const Camera&) = delete;

        void UpdateSpeed(const Input& input);

        Matrix GetViewProjectionMatrix();
        Matrix GetInvViewMatrix();
        Matrix GetInvProjectionMatrix();

        VECTOR3 GetCameraRightVector();
        VECTOR3 GetCameraUpVector();
        VECTOR3 GetCameraForwardVector();

    public:
        explicit Camera(const UINT windowWidth, const UINT windowHeight);
        ~Camera();

        void Update(const Input& input);

        void InitializeGraphicsResource(Graphics::IGraphics* pGraphics);

        void SetGraphicsResource(Graphics::IGraphics* pGraphics);

        Graphics::IDX12Resouce* GetBufferResource() const { return pGraphicsResource; }
    };

} // namespace

#endif // _CAMERA_H_