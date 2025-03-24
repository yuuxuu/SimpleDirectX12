/**
 * @file DirectionalLight.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/08
 */

#pragma once

#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include "GraphicsAPI/DirectX12/IGraphics.h"

#include "GraphicsAPI/DirectX12/DX12Resource/IDX12Resource.h"

namespace Simple 
{
namespace Light
{
    class DirectionalLight
    {
    private:
        Graphics::IDX12Resouce* pConstantBufferResource;

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

