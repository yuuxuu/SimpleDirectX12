/**
 * @file PointLight.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/08
 */

#pragma once

#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "GraphicsAPI/IGraphics.h"
#include "GraphicsAPI/IGraphicsResource.h"

#include "Buffer/LightBuffer.h"

namespace Simple 
{
    class ModelMesh;

namespace Light
{
    class PointLight
    {
    private:
        Simple::PointLightBuffer            m_pointLightBuffer;

        std::unique_ptr<Simple::ModelMesh>  pModelMesh;
    private:
        PointLight(const PointLight&) = delete;
        PointLight operator=(const PointLight&) = delete;

    public:
        explicit PointLight(const Simple::PointLightBuffer& pointLightBuffer);
        ~PointLight();

        void InitializeGraphicsResource(Graphics::IGraphics* pGraphics);

        void SetGraphicsResource(Graphics::IGraphics* pGraphics);

        void SetPointLightBuffer(const Simple::PointLightBuffer& pointLightBuffer);
        const Simple::PointLightBuffer& GetPointLightBuffer() { return m_pointLightBuffer; }
    };
} // namespace Light
} // namespace Simple

#endif // _POINT_LIGHT_H_


