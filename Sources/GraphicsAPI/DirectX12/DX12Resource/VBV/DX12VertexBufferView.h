/**
 * @file DX12VertexBufferView.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#pragma once

#ifndef _DX12_VERTEX_BUFFER_VIEW_H_
#define _DX12_VERTEX_BUFFER_VIEW_H_

#include "GraphicsAPI/DirectX12/DX12Resource/IDX12Resource.h"

#include "Param/BufferParam.h"

namespace Graphics
{
    class DX12VertexBufferView : public IDX12Resouce
    {
    private:
        ComPtr<ID3D12Resource>              m_pResource;

        D3D12_VERTEX_BUFFER_VIEW            m_VertexBufferView;

        DX12Device*                         pDX12Device;

        void*                               m_pMappedBuffer;

        Simple::BufferParam*                m_pBufferParam;

    private:
        DX12VertexBufferView(const DX12VertexBufferView&) = delete;
        DX12VertexBufferView& operator=(const DX12VertexBufferView&) = delete;

        void CreateResource(CD3DX12_HEAP_PROPERTIES prop, D3D12_RESOURCE_DESC desc) override;

        void CreateResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle) override;

        void MapResouce();

    public:
        explicit DX12VertexBufferView(DX12Device* pDX12Device, Simple::BufferParam* pParam);
        ~DX12VertexBufferView();

        void Initialize(CD3DX12_HEAP_PROPERTIES* heapProperties, D3D12_RESOURCE_DESC* resourceDesc, UINT heapIndex = 0) override;

        void UpdateResourceBuffer(const void* updateSource, Simple::IParam* pParam);

        const ID3D12Resource* GetResource() const { return m_pResource.Get(); }

        D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView() { return &m_VertexBufferView; }
    };
} // namespace

#endif // _DX12_VERTEX_BUFFER_VIEW_H_