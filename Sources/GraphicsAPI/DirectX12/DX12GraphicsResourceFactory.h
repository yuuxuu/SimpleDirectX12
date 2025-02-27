/**
 * @file DX12GraphicsResourceFactory.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#ifndef _DX12_GRAPHICS_RESOURCE_FACTORY_H_
#define _DX12_GRAPHICS_RESOURCE_FACTORY_H_

#include "Param/IParam.h"

#include "GraphicsAPI/DirectX12/DX12Resource/IDX12Resource.h"

namespace Graphics
{
    class DX12Device;
    class DX12Command;
    class DX12HeapAllocator;

    class IGraphicsResourceFactory
    {
    public:
        IGraphicsResourceFactory() {};
        virtual ~IGraphicsResourceFactory() {};

        virtual std::unique_ptr<IDX12Resouce> CreateResource(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator, Simple::IParam* pParam) = 0;
    };

    class DX12RenderTargetViewResourceFactory : public IGraphicsResourceFactory
    {
    public:
        DX12RenderTargetViewResourceFactory(){}
        ~DX12RenderTargetViewResourceFactory(){}

        std::unique_ptr<IDX12Resouce> CreateResource(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator, Simple::IParam* pParam) override;
    };

    class DX12DepthStencilViewResourceFactory : public IGraphicsResourceFactory
    {
    public:
        DX12DepthStencilViewResourceFactory(){}
        ~DX12DepthStencilViewResourceFactory(){}

        std::unique_ptr<IDX12Resouce> CreateResource(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator, Simple::IParam* pParam) override;
    };

    class DX12ConstantBufferViewResourceFactory : public IGraphicsResourceFactory
    {
    public:
        DX12ConstantBufferViewResourceFactory() {}
        ~DX12ConstantBufferViewResourceFactory() {}

        std::unique_ptr<IDX12Resouce> CreateResource(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator, Simple::IParam* pParam) override;
    };

    class DX12VertexBufferResourceFactory : public IGraphicsResourceFactory
    {
    public:
        DX12VertexBufferResourceFactory() {}
        ~DX12VertexBufferResourceFactory() {}

        std::unique_ptr<IDX12Resouce> CreateResource(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator, Simple::IParam* pParam) override;
    };

    class DX12IndexBufferResourceFactory : public IGraphicsResourceFactory
    {
    public:
        DX12IndexBufferResourceFactory() {}
        ~DX12IndexBufferResourceFactory() {}

        std::unique_ptr<IDX12Resouce> CreateResource(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator, Simple::IParam* pParam) override;
    };

    class DX12ShaderResourceViewResourceFactory : public IGraphicsResourceFactory
    {
    private:
        DX12Command* pDX12Command;

    public:
        explicit DX12ShaderResourceViewResourceFactory(DX12Command* pDX12Command);
        ~DX12ShaderResourceViewResourceFactory() {}

        std::unique_ptr<IDX12Resouce> CreateResource(DX12Device* pDX12Device, DX12HeapAllocator* pHeapAllocator, Simple::IParam* pParam) override;
    };
} // namespace

#endif // _DX12_GRAPHICS_RESOURCE_FACTORY_H_
