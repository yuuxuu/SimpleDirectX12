/**
 * @file DX12GraphicsResourceBuilder.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#ifndef _DX12_GRAPHICS_RESOURCE_BUILDER_H_
#define _DX12_GRAPHICS_RESOURCE_BUILDER_H_

#include "GraphicsAPI/DirectX12/DX12Resource/IDX12Resource.h"

namespace Graphics
{
    class DX12GraphicsResourceBuilder
    {
    private:
        DX12GraphicsResourceBuilder(const DX12GraphicsResourceBuilder&) = delete;
        DX12GraphicsResourceBuilder& operator=(const DX12GraphicsResourceBuilder) = delete;

    public:
        DX12GraphicsResourceBuilder();
        ~DX12GraphicsResourceBuilder();

        void CreateRenderTargetView(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator,UINT windowWidth, UINT windowHeight, std::unique_ptr<IDX12Resouce>& pGraphicsResource);
        
        void CreateDepthStencilView(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator,UINT windowWidth, UINT windowHeight, std::unique_ptr<IDX12Resouce>& pGraphicsResource);

        void CreateConstantBufferView(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator, UINT byteWidth,std::unique_ptr<IDX12Resouce>& pGraphicsResource);
    };
} // namespace

#endif // _DX12_GRAPHICS_RESOURCE_BUILDER_H_
