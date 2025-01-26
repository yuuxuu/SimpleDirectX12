/**
 * @file DX12ShaderResourceView.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#include "DX12ShaderResourceView.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12Command.h"
#include "GraphicsAPI/DirectX12/DX12HeapAllocator.h"

namespace Graphics
{
    DX12ShaderResourceView::DX12ShaderResourceView(DX12Device* pDX12Device, DX12Command* pDX12Command, DX12HeapAllocator* pDX12HeapAllocator, D3D12_SHADER_RESOURCE_VIEW_DESC* shaderResourceViewDesc, D3D12_SUBRESOURCE_DATA* subData) :
        pDX12Device(pDX12Device),
        pDX12Command(pDX12Command),
        pDX12HeapAllocator(pDX12HeapAllocator),
        m_pShaderSesourceViewDesc(shaderResourceViewDesc),
        m_pSubData(subData),
        m_descriptorHandle()
    {}

    DX12ShaderResourceView::~DX12ShaderResourceView()
    {}

    void DX12ShaderResourceView::CreateResource(CD3DX12_HEAP_PROPERTIES prop,D3D12_RESOURCE_DESC desc)
    {
        pDX12Device->CreateResource(m_pResource, prop, desc, nullptr, D3D12_RESOURCE_STATE_COPY_DEST);
    }

    void DX12ShaderResourceView::CreateResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
    {
        pDX12Device->CreateShaderResourceView(m_pResource.Get(), *m_pShaderSesourceViewDesc, descriptorHandle);
    }

    void DX12ShaderResourceView::UpdateSubresource()
    {
        auto pCommandList = pDX12Command->GetCommandList();
        if (!pCommandList)
        {
            MessageBoxA(NULL, "CommandListがnullptrです。", "MessageBox", MB_OK);
            return;
        }

        UINT64 bufSize = GetRequiredIntermediateSize(m_pResource.Get(), 0, 1);
        CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufSize);
        auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        ComPtr<ID3D12Resource> resource;
        pDX12Device->CreateResource(resource, prop, desc);

        D3D12_SUBRESOURCE_DATA subData = {};

        pDX12Command->ResetCommandList();

         UpdateSubresources(
             pCommandList,
            m_pResource.Get(),
            resource.Get(),
            0,
            0,
            1,
            m_pSubData);

        auto rBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            m_pResource.Get(),
            D3D12_RESOURCE_STATE_COPY_DEST,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

        pCommandList->ResourceBarrier(1, &rBarrier);

        pDX12Command->ExecuteComandLists();
    }

    void DX12ShaderResourceView::Initialize(
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
            UpdateSubresource();

            m_descriptorHandle = pDX12HeapAllocator->GetCPUDescriptorHeapHandle(heapIndex);
            CreateResourceView(m_descriptorHandle);
        }
    }

} // namespace