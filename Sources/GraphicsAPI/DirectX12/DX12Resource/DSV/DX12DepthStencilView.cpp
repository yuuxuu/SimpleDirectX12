/**
 * @file DX12DepthStencilView.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#include "DX12DepthStencilView.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12Command.h"
#include "GraphicsAPI/DirectX12/DX12HeapAllocator.h"

namespace Graphics
{
    DX12DepthStencilView::DX12DepthStencilView(DX12Device* pDX12Device, DX12HeapAllocator* pDX12HeapAllocator, D3D12_DEPTH_STENCIL_VIEW_DESC* depthStencilDesc) :
        pDX12Device(pDX12Device),
        pDX12HeapAllocator(pDX12HeapAllocator),
        m_pDepthStencilViewDesc(depthStencilDesc),
        m_descriptorHandle()
    {}

    DX12DepthStencilView::~DX12DepthStencilView()
    {}

    void DX12DepthStencilView::CreateResource(CD3DX12_HEAP_PROPERTIES prop,D3D12_RESOURCE_DESC desc)
    {
        D3D12_CLEAR_VALUE clearValue = {};

        clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        clearValue.DepthStencil.Depth = 1.0f;
        clearValue.DepthStencil.Stencil = 0;

        pDX12Device->CreateResource(m_pResource, prop, desc, &clearValue);
    }

    void DX12DepthStencilView::CreateResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
    {
        pDX12Device->CreateDepthStencilView(m_pResource.Get(), m_pDepthStencilViewDesc, descriptorHandle);
    }

    void DX12DepthStencilView::Initialize(
        CD3DX12_HEAP_PROPERTIES* heapProperties, 
        D3D12_RESOURCE_DESC* resourceDesc,
        UINT heapIndex)
    {
        CreateResource(*heapProperties, *resourceDesc);

        if (m_pResource)
        {
            m_descriptorHandle = pDX12HeapAllocator->GetDescriptorHeapHandle(heapIndex);
            CreateResourceView(m_descriptorHandle);
        }
    }

} // namespace