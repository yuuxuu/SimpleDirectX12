/**
 * @file DX12VertexBufferView.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#include "DX12VertexBufferView.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12Command.h"
#include "GraphicsAPI/DirectX12/DX12HeapAllocator.h"

#include "Param/BufferParam.h"

namespace Graphics
{
    DX12VertexBufferView::DX12VertexBufferView(DX12Device* pDX12Device, Simple::IParam* pParam) :
        pDX12Device(pDX12Device),
        m_VertexBufferView(),
        m_pMappedBuffer(),
        m_pParam(dynamic_cast<Simple::BufferParam*>(pParam))
    {}

    DX12VertexBufferView::~DX12VertexBufferView()
    {
        if(m_pResource)
            m_pResource->Unmap(0, nullptr);
    }

    void DX12VertexBufferView::CreateResource(CD3DX12_HEAP_PROPERTIES prop,D3D12_RESOURCE_DESC desc)
    {
        pDX12Device->CreateResource(m_pResource, prop, desc);
    }

    void DX12VertexBufferView::CreateResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
    {
    }

    void DX12VertexBufferView::MapResouce()
    {
        D3D12_RANGE range = { 0, 0 };

        HRESULT hr = m_pResource->Map(0, &range, &m_pMappedBuffer);
        if (FAILED(hr)) {
            m_pResource->Unmap(0, nullptr);

            MessageBoxA(NULL, "ResouceのMap：失敗", "MessageBox", MB_OK);
            return;
        }
    }

    void DX12VertexBufferView::Initialize(
        CD3DX12_HEAP_PROPERTIES* heapProperties, 
        D3D12_RESOURCE_DESC* resourceDesc,
        UINT heapIndex)
    {
        if (heapProperties && resourceDesc)
            CreateResource(*heapProperties, *resourceDesc);

        if (m_pResource)
        {
            MapResouce();

            m_VertexBufferView.BufferLocation = m_pResource->GetGPUVirtualAddress();
            m_VertexBufferView.SizeInBytes = m_pParam->byteWidth;
            m_VertexBufferView.StrideInBytes = m_pParam->byteWidthStride;
        }
    }

    void DX12VertexBufferView::UpdateResourceBuffer(const void* pSource)
    {
        memcpy(m_pMappedBuffer, pSource, m_pParam->byteWidth);
    }

} // namespace