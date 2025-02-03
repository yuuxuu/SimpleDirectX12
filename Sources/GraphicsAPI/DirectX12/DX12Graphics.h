/**
 * @file DX12Graphics.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#ifndef _DX12_GRAPHICS_H_
#define _DX12_GRAPHICS_H_

#include "GraphicsAPI/IGraphics.h"

#include "GraphicsAPI/DirectX12/DX12Resource/IDX12Resource.h"

#include "GraphicsAPI/Shader/DirectX/GraphicsPipeline.h"

#include "Param/IParam.h"

namespace Graphics
{
    namespace Shader
    {
        class Shader;
    }

    using HeapAllocatorMap = std::map<D3D12_DESCRIPTOR_HEAP_TYPE, std::unique_ptr<DX12HeapAllocator>>;
    using GraphicsPipelineVec = std::vector<std::unique_ptr<Shader::GraphicsPipeline>>;
    using ShaderVec = std::vector<std::unique_ptr<Shader::Shader>>;

    class DX12Device;
    class DX12Command;
    class DX12HeapAllocator;

    class DX12Graphics : public IGraphics
    {
    private:
        std::unique_ptr<DX12Device>                     m_pDX12Device;
        std::unique_ptr<DX12Command>                    m_pDX12Command;

        GraphicsPipelineVec                             m_pGraphicsPipelineVec;
        ShaderVec                                       m_pShaderVec;

        HeapAllocatorMap                                m_pDX12HeapAllocatorMap;

        std::vector<std::unique_ptr<IDX12Resouce>>      m_pRenderTargetViews;
        std::unique_ptr<IDX12Resouce>                   m_pDepthStencilView;

        std::vector<std::unique_ptr<IDX12Resouce>>      m_pGraphicsBufferResourceViews;

    private:
        DX12Graphics(const DX12Graphics&) = delete;
        DX12Graphics& operator=(const DX12Graphics) = delete;

    public:
        DX12Graphics();
        ~DX12Graphics();

        bool Initialize(HWND hwnd, const UINT windowWidth, const UINT windowHeight) override;

        void Finalize() override;

        void Update(const UINT windowWidth, const UINT windowHeight) override;

        void InitializeGraphicsPipeline() override;

        void SetGraphicsPipeline() override;

        void InitializeGraphicsBufferResource(IGraphicsResource*& pGraphicsResource, Simple::IParam* pParam, GraphicsResourceType graphicsResourceType) override;

        void UpdateGraphicsBufferResource(IGraphicsResource* pGraphicsResource, const void* updateSource, GraphicsResourceType graphicsResourceType) override;

        void SetConstantBufferResource(UINT index, IGraphicsResource* pGraphicsResource) override;

        void SetShaderResource(UINT index, IGraphicsResource* pGraphicsResource) override;

        void DrawInstancedVertexBuffer(IGraphicsResource* pVertexResource, UINT numVerties) override;

        void DrawIndexedIndexBuffer(IGraphicsResource* pVertexResource, IGraphicsResource* pIndexResource, UINT numIndeies) override;
    };
} // namespace

#endif // _DX12_GRAPHICS_H_
