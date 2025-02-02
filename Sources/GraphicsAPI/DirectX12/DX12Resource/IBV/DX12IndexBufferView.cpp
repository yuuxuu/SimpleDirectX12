/**
 * @file DX12IndexBufferView.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#include "DX12IndexBufferView.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12Command.h"
#include "GraphicsAPI/DirectX12/DX12HeapAllocator.h"

#include "Param/BufferParam.h"

namespace Graphics
{
    DX12IndexBufferView::DX12IndexBufferView(DX12Device* pDX12Device, Simple::IParam* pParam) :
        pDX12Device(pDX12Device),
        m_IndexBufferView(),
        m_pMappedBuffer(),
        m_pParam(dynamic_cast<Simple::BufferParam*>(pParam))
    {}

    DX12IndexBufferView::~DX12IndexBufferView()
    {
        if(m_pResource)
            m_pResource->Unmap(0, nullptr);
    }

    void DX12IndexBufferView::CreateResource(CD3DX12_HEAP_PROPERTIES prop,D3D12_RESOURCE_DESC desc)
    {
        pDX12Device->CreateResource(m_pResource, prop, desc);
    }

    void DX12IndexBufferView::CreateResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
    {
    }

    void DX12IndexBufferView::MapResouce()
    {
        D3D12_RANGE range = { 0, 0 };

        HRESULT hr = m_pResource->Map(0, &range, &m_pMappedBuffer);
        if (FAILED(hr)) {
            m_pResource->Unmap(0, nullptr);

            MessageBoxA(NULL, "ResouceのMap：失敗", "MessageBox", MB_OK);
            return;
        }
    }

    void DX12IndexBufferView::Initialize(
        CD3DX12_HEAP_PROPERTIES* heapProperties, 
        D3D12_RESOURCE_DESC* resourceDesc,
        UINT heapIndex)
    {
        if (heapProperties && resourceDesc)
            CreateResource(*heapProperties, *resourceDesc);

        if (m_pResource)
        {
            MapResouce();

            m_IndexBufferView.BufferLocation = m_pResource->GetGPUVirtualAddress();
            m_IndexBufferView.SizeInBytes = m_pParam->byteWidth;
            m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
        }
    }

    void DX12IndexBufferView::UpdateResourceBuffer(const void* pSource)
    {
        memcpy(m_pMappedBuffer, pSource, m_pParam->byteWidth);
    }

} // namespace