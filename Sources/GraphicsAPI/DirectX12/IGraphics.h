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

#include "GraphicsResourceType.h"

namespace Graphics
{
    class IDX12Resouce;

    class IGraphics
    {
    public:
        IGraphics() {}
        virtual ~IGraphics() {}

        virtual bool Initialize(HWND hwnd, UINT width, UINT height) = 0;

        virtual void Finalize() = 0;

        virtual void Update() = 0;

        virtual void InitializeGraphicsPipeline() = 0;

        virtual void SetGraphicsPipeline() = 0;

        virtual void InitializeGraphicsBufferResource(IDX12Resouce*& pGraphicsResource, Simple::IParam* param, GraphicsResourceType GraphicsResourceType) = 0;

        virtual void UpdateGraphicsBufferResource(IDX12Resouce* pGraphicsResource, const void* updateSource, Simple::IParam* pParam, GraphicsResourceType graphicsResourceType) = 0;

        virtual void SetConstantBufferResource(UINT index, IDX12Resouce* pGraphicsResource) = 0;

        virtual void SetShaderResource(UINT index, IDX12Resouce* pGraphicsResource) = 0;

        virtual void DrawInstancedVertexBuffer(IDX12Resouce* pVertexResource, UINT numVerties) = 0;

        virtual void DrawIndexedIndexBuffer(IDX12Resouce* pVertexResource, IDX12Resouce* pIndexResource, UINT numIndeies) = 0;
    };
} // namespace

#endif // _IGRAPHICS_H_

