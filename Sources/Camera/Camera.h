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

#include "GraphicsAPI/IGraphics.h"
#include "GraphicsAPI/IGraphicsResource.h"

namespace Simple 
{
    class Camera 
    {
    private:
        Graphics::IGraphicsResource*    m_pGraphicsResource;

        VECTOR3                         m_eye;
        VECTOR3                         m_forcus;
        VECTOR3                         m_up;

        Matrix                          m_matView;
        Matrix                          m_matProj;

        float                           m_speed;

    private:
        Camera(const Camera&) = delete;
        Camera operator=(const Camera&) = delete;

        Matrix GetViewProjectionMatrix();
        Matrix GetInvViewMatrix();
        Matrix GetInvProjectionMatrix();

        VECTOR3 GetCameraRightVector();
        VECTOR3 GetCameraUpVector();
        VECTOR3 GetCameraForwardVector();

    public:
        Camera(const UINT windowWidth, const UINT windowHeight);
        ~Camera();

        void InitializeGraphicsResource(Graphics::IGraphics* pGraphics);
    };

} // namespace

#endif // _CAMERA_H_