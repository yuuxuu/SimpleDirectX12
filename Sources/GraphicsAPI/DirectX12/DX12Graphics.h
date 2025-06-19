/**
 * @file DX12Graphics.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#include "IGraphics.h"

#include "GraphicsAPI/DirectX12/DX12Resource/IDX12Resource.h"

#include "GraphicsAPI/DirectX12/DX12Pipline/PSO/IDX12PSO.h"

#include "Param/IParam.h"

namespace Graphics
{
    namespace Shader
    {
        class Shader;
    }

    using HeapAllocatorMap = std::map<D3D12_DESCRIPTOR_HEAP_TYPE, std::unique_ptr<DX12HeapAllocator>>;
    using GraphicsPipelineVec = std::vector<std::unique_ptr<IDX12PSO>>;
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

        UINT                                            m_windowWidth;
        UINT                                            m_windowHeight;

    private:
        DX12Graphics(const DX12Graphics&) = delete;
        DX12Graphics& operator=(const DX12Graphics) = delete;

    public:
        DX12Graphics();
        ~DX12Graphics();

        bool Initialize(HWND hwnd, const UINT windowWidth, const UINT windowHeight) override;

        void Finalize() override;

        void Update() override;

        void InitializeGraphicsPipeline() override;

        void SetGraphicsPipeline() override;

        void InitializeGraphicsBufferResource(IDX12Resouce*& pGraphicsResource, Simple::IParam* param, GraphicsResourceType graphicsResourceType) override;

        void UpdateGraphicsBufferResource(IDX12Resouce* pGraphicsResource, const void* updateSource, Simple::IParam* pParam, GraphicsResourceType graphicsResourceType) override;

        void SetConstantBufferResource(UINT index, IDX12Resouce* pGraphicsResource) override;

        void SetShaderResource(UINT index, IDX12Resouce* pGraphicsResource) override;

        void DrawInstancedVertexBuffer(IDX12Resouce* pVertexResource, UINT numVerties) override;

        void DrawIndexedIndexBuffer(IDX12Resouce* pVertexResource, IDX12Resouce* pIndexResource, UINT numIndies) override;
    };
} // namespace
