/**
 * @file DX12GraphicsResourceBuilder.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#include "DX12GraphicsResourceBuilder.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12HeapAllocator.h"

#include "GraphicsAPI/DirectX12/DX12Resource/RTV/DX12RenderTargetView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/DSV/DX12DepthStencilView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/CBV/DX12ConstantBurfferView.h"

namespace Graphics
{
    DX12GraphicsResourceBuilder::DX12GraphicsResourceBuilder()
    {}

    DX12GraphicsResourceBuilder::~DX12GraphicsResourceBuilder()
    {}

    void DX12GraphicsResourceBuilder::CreateRenderTargetView(
        DX12Device* pDX12Device, 
        DX12HeapAllocator* pHeapAllocator, 
        UINT windowWidth, 
        UINT windowHeight,
        std::unique_ptr<IDX12Resouce>& pDX12Resource)
    {
        auto renderTargetView = std::make_unique<DX12RenderTargetView>(pDX12Device, pHeapAllocator, nullptr);
        renderTargetView->Initialize(nullptr, nullptr, pHeapAllocator->GetHeapIndex());

        pDX12Resource = std::move(renderTargetView);
    }

    void DX12GraphicsResourceBuilder::CreateDepthStencilView(
        DX12Device* pDX12Device, 
        DX12HeapAllocator* pHeapAllocator,
        UINT windowWidth, 
        UINT windowHeight,
        std::unique_ptr<IDX12Resouce>& pDX12Resource)
    {
        D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilviewDesc = {};

        depthStencilviewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilviewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        depthStencilviewDesc.Flags = D3D12_DSV_FLAG_NONE;
        depthStencilviewDesc.Texture2D.MipSlice = 0;

        D3D12_RESOURCE_DESC resourceDesc = {};

        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        resourceDesc.Width = windowWidth;
        resourceDesc.Height = windowHeight;
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
        resourceDesc.SampleDesc = { 1 , 0 };
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

        auto pDepthStencilView = std::make_unique<DX12DepthStencilView>(pDX12Device,pHeapAllocator, &depthStencilviewDesc);
        pDepthStencilView->Initialize(&prop, &resourceDesc);

        pDX12Resource = std::move(pDepthStencilView);
    }

    void DX12GraphicsResourceBuilder::CreateConstantBufferView(
        DX12Device* pDX12Device,
        DX12HeapAllocator* pHeapAllocator,
        UINT byteWidth,
        std::unique_ptr<IDX12Resouce>& pDX12Resource)
    {
        D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferViewDesc = {};

        constantBufferViewDesc.SizeInBytes = (byteWidth + 0xff) & ~0xff;

        D3D12_RESOURCE_DESC resourceDesc = {};

        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resourceDesc.Width = (byteWidth + 0xff) & ~0xff;
        resourceDesc.Height = 1;
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        resourceDesc.SampleDesc.Count = 1;

        CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        auto pConstantBufferView = std::make_unique<DX12ConstantBufferView>(pDX12Device, pHeapAllocator, &constantBufferViewDesc);
        pConstantBufferView->Initialize(&prop, &resourceDesc, pHeapAllocator->GetHeapIndex());

        pHeapAllocator->AddHeapIndex();

        pDX12Resource = std::move(pConstantBufferView);
    }

} // namespace