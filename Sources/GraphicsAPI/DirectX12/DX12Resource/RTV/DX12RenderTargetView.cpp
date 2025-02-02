/**
 * @file DX12RenderTargetView.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#include "DX12RenderTargetView.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12Command.h"
#include "GraphicsAPI/DirectX12/DX12HeapAllocator.h"

namespace Graphics
{
    DX12RenderTargetView::DX12RenderTargetView(DX12Device* pDX12Device, DX12HeapAllocator* pDX12HeapAllocator, D3D12_RENDER_TARGET_VIEW_DESC* renderTargetViewDesc) :
        pDX12Device(pDX12Device),
        pDX12HeapAllocator(pDX12HeapAllocator),
        m_pRenderTargetViewDesc(renderTargetViewDesc),
        m_CPUDescriptorHandle(),
        m_GPUDescriptorHandle()
    {}

    DX12RenderTargetView::~DX12RenderTargetView()
    {}

    void DX12RenderTargetView::CreateResource(CD3DX12_HEAP_PROPERTIES prop,D3D12_RESOURCE_DESC desc)
    {
        D3D12_CLEAR_VALUE clearValue = {};

        clearValue.Format = desc.Format;
        clearValue.Color[0] = 0.0f;
        clearValue.Color[1] = 0.0f;
        clearValue.Color[2] = 0.0f;
        clearValue.Color[3] = 1.0f;

        pDX12Device->CreateResource(m_pResource, prop, desc, &clearValue);
    }

    void DX12RenderTargetView::CreateResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
    {
        pDX12Device->CreateRenderTargetView(m_pResource.Get(), m_pRenderTargetViewDesc, descriptorHandle);
    }

    void DX12RenderTargetView::Initialize(
        CD3DX12_HEAP_PROPERTIES* heapProperties, 
        D3D12_RESOURCE_DESC* resourceDesc,
        UINT heapIndex)
    {
        if(heapProperties && resourceDesc)
            CreateResource(*heapProperties, *resourceDesc);
        else
            pDX12Device->GetBackBuffer(m_pResource, heapIndex);

        if (m_pResource)
        {
            m_CPUDescriptorHandle = pDX12HeapAllocator->GetCPUDescriptorHeapHandle(heapIndex);

            if (pDX12HeapAllocator->IsVisibleShader())
                m_GPUDescriptorHandle = pDX12HeapAllocator->GetGPUDescriptorHeapHandle(heapIndex);
            
            CreateResourceView(m_CPUDescriptorHandle);
        }
    }

} // namespace