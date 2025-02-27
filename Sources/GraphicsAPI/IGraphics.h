/**
 * @file IGraphics.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#ifndef _IGRAPHICS_H_
#define _IGRAPHICS_H_

#include "Param/IParam.h"

#include "GraphicsAPI/GraphicsResourceType.h"

namespace Graphics
{
    class IGraphicsResource;
    class IGraphics
    {
    public:
        IGraphics() {}
        virtual ~IGraphics() {}

        virtual bool Initialize(HWND hwnd, UINT width, UINT height) = 0;

        virtual void RenderPrepare() = 0;

        virtual void RenderEnd() = 0;

        virtual void InitializeGraphicsPipeline() = 0;

        virtual void SetGraphicsPipeline() = 0;

        virtual void InitializeGraphicsBufferResource(IGraphicsResource*& pGraphicsResource, Simple::IParam* param, GraphicsResourceType GraphicsResourceType) = 0;

        virtual void UpdateGraphicsBufferResource(IGraphicsResource* pGraphicsResource, const void* updateSource, Simple::IParam* pParam, GraphicsResourceType graphicsResourceType) = 0;

        virtual void SetConstantBufferView(UINT index, IGraphicsResource* pGraphicsResource) = 0;

        virtual void SetShaderResourceView(UINT index, IGraphicsResource* pGraphicsResource) = 0;

        virtual void DrawInstancedVertexBuffer(IGraphicsResource* pVertexResource, UINT numVerties) = 0;

        virtual void DrawIndexedIndexBuffer(IGraphicsResource* pVertexResource, IGraphicsResource* pIndexResource, UINT numIndeies) = 0;
    };
} // namespace

#endif // _IGRAPHICS_H_

