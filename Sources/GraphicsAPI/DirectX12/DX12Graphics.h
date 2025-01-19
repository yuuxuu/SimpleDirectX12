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

namespace Graphics
{
    class DX12Device;
    class DX12Command;
    class DX12HeapAllocator;
    class DX12GraphicsResourceBuilder;

    using HeapAllocatorMap = std::map<D3D12_DESCRIPTOR_HEAP_TYPE, std::unique_ptr<DX12HeapAllocator>>;

    class DX12Graphics : public IGraphics
    {
    private:
        std::unique_ptr<DX12Device>                     m_pDX12Device;
        std::unique_ptr<DX12Command>                    m_pDX12Command;
        HeapAllocatorMap                                m_pDX12HeapAllocatorMap;
        std::unique_ptr<DX12GraphicsResourceBuilder>    m_pDX12GraphicsResourceBuilder;

        std::vector<std::unique_ptr<IDX12Resouce>>      m_pRenderTargetViews;
        std::unique_ptr<IDX12Resouce>                   m_pDepthStencilView;

        std::vector<std::unique_ptr<IDX12Resouce>>      m_pConstantBufferViews;

    private:
        DX12Graphics(const DX12Graphics&) = delete;
        DX12Graphics& operator=(const DX12Graphics) = delete;

    public:
        DX12Graphics();
        ~DX12Graphics();

        bool InitializeGraphics(HWND hwnd, const UINT windowWidth, const UINT windowHeight) override;

        void FinalizeGraphics() override;

        void UpdateGraphics(const UINT windowWidth, const UINT windowHeight) override;

        void InitializeGraphicsResource(IGraphicsResource*& pGraphicsResource, const UINT byteWidth) override;

        void UpdateGraphicsResource(IGraphicsResource* pGraphicsResource, const void* updateSource) override;
    };
} // namespace

#endif // _DX12_GRAPHICS_H_
