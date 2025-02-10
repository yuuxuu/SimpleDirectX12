/**
 * @file DirectionalLight.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/08
 */

#pragma once

#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include "GraphicsAPI/IGraphics.h"
#include "GraphicsAPI/IGraphicsResource.h"

namespace Simple 
{
namespace Light
{
    class DirectionalLight
    {
    private:
        Graphics::IGraphicsResource* pConstantBufferResource;

    private:
        DirectionalLight(const DirectionalLight&) = delete;
        DirectionalLight operator=(const DirectionalLight&) = delete;

    public:
        DirectionalLight();
        ~DirectionalLight();

        void InitializeGraphicsResource(Graphics::IGraphics* pGraphics);

        void SetGraphicsResource(Graphics::IGraphics* pGraphics);
    };
} // namespace Light
} // namespace Simple

#endif // _DIRECTIONAL_LIGHT_H_

