/**
 * @file DX12ConstantBufferView.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#include "DX12ConstantBurfferView.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12Command.h"
#include "GraphicsAPI/DirectX12/DX12HeapAllocator.h"

namespace Graphics
{
    DX12ConstantBufferView::DX12ConstantBufferView(DX12Device* pDX12Device, DX12HeapAllocator* pDX12HeapAllocator, D3D12_CONSTANT_BUFFER_VIEW_DESC* constantBufferViewDesc) :
        pDX12Device(pDX12Device),
        pDX12HeapAllocator(pDX12HeapAllocator),
        m_pConstantBufferViewDesc(constantBufferViewDesc),
        m_descriptorHandle(),
        m_pMappedBuffer(),
        m_SizeInBytes()
    {}

    DX12ConstantBufferView::~DX12ConstantBufferView()
    {
        if(m_pResource)
            m_pResource->Unmap(0, nullptr);
    }

    void DX12ConstantBufferView::CreateResource(CD3DX12_HEAP_PROPERTIES prop,D3D12_RESOURCE_DESC desc)
    {
        pDX12Device->CreateResource(m_pResource, prop, desc);
    }

    void DX12ConstantBufferView::CreateResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
    {
        pDX12Device->CreateConstantBufferView(m_pResource.Get(), *m_pConstantBufferViewDesc, descriptorHandle);
    }

    void DX12ConstantBufferView::MapResouce()
    {
        D3D12_RANGE range = { 0, 0 };

        HRESULT hr = m_pResource->Map(0, &range, &m_pMappedBuffer);
        if (FAILED(hr)) {
            m_pResource->Unmap(0, nullptr);

            MessageBoxA(NULL, "ResouceのMap：失敗", "MessageBox", MB_OK);
            return;
        }
    }

    void DX12ConstantBufferView::Initialize(
        CD3DX12_HEAP_PROPERTIES* heapProperties, 
        D3D12_RESOURCE_DESC* resourceDesc,
        UINT heapIndex)
    {
        if (heapProperties && resourceDesc)
        {
            CreateResource(*heapProperties, *resourceDesc);
        }

        if (m_pResource)
        {
            MapResouce();

            m_SizeInBytes = m_pConstantBufferViewDesc->SizeInBytes;
            m_pConstantBufferViewDesc->BufferLocation = m_pResource->GetGPUVirtualAddress();

            m_descriptorHandle = pDX12HeapAllocator->GetCPUDescriptorHeapHandle(heapIndex);
            CreateResourceView(m_descriptorHandle);
        }
    }

    void DX12ConstantBufferView::UpdateResourceBuffer(const void* pSource)
    {
        memcpy(m_pMappedBuffer, pSource, m_SizeInBytes);
    }

} // namespace