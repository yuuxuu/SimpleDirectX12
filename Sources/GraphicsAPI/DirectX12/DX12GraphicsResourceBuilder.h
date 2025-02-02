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

#include "Param/IParam.h"

namespace Graphics
{
    class Texture;

    class DX12GraphicsResourceBuilder
    {
    private:
        DX12GraphicsResourceBuilder();
        ~DX12GraphicsResourceBuilder();

        DX12GraphicsResourceBuilder(const DX12GraphicsResourceBuilder&) = delete;
        DX12GraphicsResourceBuilder& operator=(const DX12GraphicsResourceBuilder) = delete;

    public:

        static void CreateRenderTargetView(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator,Simple::IParam* pParam, std::unique_ptr<IDX12Resouce>& pGraphicsResource);
        
        static void CreateDepthStencilView(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator, Simple::IParam* pParam, std::unique_ptr<IDX12Resouce>& pGraphicsResource);

        static void CreateConstantBufferView(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator, Simple::IParam* pParam,std::unique_ptr<IDX12Resouce>& pGraphicsResource);

        static void CreateVertexBufferView(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator, Simple::IParam* pParam,std::unique_ptr<IDX12Resouce>& pGraphicsResource);
        
        static void CreateIndexBufferView(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator, Simple::IParam* pParam,std::unique_ptr<IDX12Resouce>& pGraphicsResource);
        
        static void CreateShaderResourceView(DX12Device* pDX12Device, DX12Command* pDX12Command, DX12HeapAllocator* pHeapAllocator, Simple::IParam* pParam, std::unique_ptr<IDX12Resouce>& pGraphicsResource);
    };
} // namespace

#endif // _DX12_GRAPHICS_RESOURCE_BUILDER_H_
