/**
 * @file DX12ConstantBufferView.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#pragma once

#ifndef _DX12_CONSTANT_BUFFER_VIEW_H_
#define _DX12_CONSTANT_BUFFER_VIEW_H_

#include "GraphicsAPI/DirectX12/DX12Resource/IDX12Resource.h"

namespace Graphics
{
    class DX12ConstantBufferView : public IDX12Resouce
    {
    private:
        ComPtr<ID3D12Resource>              m_pResource;

        D3D12_CPU_DESCRIPTOR_HANDLE         m_descriptorHandle;

        D3D12_CONSTANT_BUFFER_VIEW_DESC*    m_pConstantBufferViewDesc = nullptr;

        DX12Device*                         pDX12Device;

        DX12HeapAllocator*                  pDX12HeapAllocator;

        void*                               m_pMappedBuffer;

        UINT                                m_SizeInBytes;

    private:
        DX12ConstantBufferView(const DX12ConstantBufferView&) = delete;
        DX12ConstantBufferView& operator=(const DX12ConstantBufferView&) = delete;

        void CreateResource(CD3DX12_HEAP_PROPERTIES prop, D3D12_RESOURCE_DESC desc) override;

        void CreateResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle) override;

    private:
        void MapResouce();

    public:
        DX12ConstantBufferView(DX12Device* pDX12Device, DX12HeapAllocator* pDX12HeapAllocator, D3D12_CONSTANT_BUFFER_VIEW_DESC* constantBufferViewDesc);
        ~DX12ConstantBufferView();

        void Initialize(CD3DX12_HEAP_PROPERTIES* heapProperties, D3D12_RESOURCE_DESC* resourceDesc, UINT heapIndex) override;

    public:
        void UpdateResourceBuffer(const void* updateSource);

        ID3D12Resource* GetResouce() const { return m_pResource.Get(); }

        D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle() const { return m_descriptorHandle; }
    };
} // namespace

#endif // _DX12_CONSTANT_BUFFER_VIEW_H_