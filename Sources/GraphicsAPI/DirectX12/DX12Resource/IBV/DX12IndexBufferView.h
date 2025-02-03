/**
 * @file DX12IndexBufferView.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#pragma once

#ifndef _DX12_INDEX_BUFFER_VIEW_H_
#define _DX12_INDEX_BUFFER_VIEW_H_

#include "GraphicsAPI/DirectX12/DX12Resource/IDX12Resource.h"

#include "Param/BufferParam.h"

namespace Graphics
{
    class DX12IndexBufferView : public IDX12Resouce
    {
    private:
        ComPtr<ID3D12Resource>              m_pResource;

        D3D12_INDEX_BUFFER_VIEW             m_IndexBufferView;

        DX12Device*                         pDX12Device;

        void*                               m_pMappedBuffer;

        Simple::BufferParam*                m_pParam;

    private:
        DX12IndexBufferView(const DX12IndexBufferView&) = delete;
        DX12IndexBufferView& operator=(const DX12IndexBufferView&) = delete;

        void CreateResource(CD3DX12_HEAP_PROPERTIES prop, D3D12_RESOURCE_DESC desc) override;

        void CreateResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle) override;

        void MapResouce();

    public:
        explicit DX12IndexBufferView(DX12Device* pDX12Device, Simple::IParam* pParam);
        ~DX12IndexBufferView();

        void Initialize(CD3DX12_HEAP_PROPERTIES* heapProperties, D3D12_RESOURCE_DESC* resourceDesc, UINT heapIndex = 0) override;

        void UpdateResourceBuffer(const void* updateSource);

        const ID3D12Resource* GetResource() const { return m_pResource.Get(); }

        D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView() { return &m_IndexBufferView; }
    };
} // namespace

#endif // _DX12_INDEX_BUFFER_VIEW_H_