/**
 * @file DX12ShaderResourceView.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#pragma once

#ifndef _DX12_SHADER_RESOURCE_VIEW_H_
#define _DX12_SHADER_RESOURCE_VIEW_H_

#include "GraphicsAPI/DirectX12/DX12Resource/IDX12Resource.h"

namespace Graphics
{
    class DX12ShaderResourceView : public IDX12Resouce
    {
    private:
        ComPtr<ID3D12Resource>              m_pResource;

        D3D12_CPU_DESCRIPTOR_HANDLE         m_descriptorHandle;

        D3D12_SHADER_RESOURCE_VIEW_DESC*    m_pShaderSesourceViewDesc = nullptr;

        D3D12_SUBRESOURCE_DATA*             m_pSubData = nullptr;

        DX12Device*                         pDX12Device;

        DX12Command*                        pDX12Command;

        DX12HeapAllocator*                  pDX12HeapAllocator;

    private:
        DX12ShaderResourceView(const DX12ShaderResourceView&) = delete;
        DX12ShaderResourceView& operator=(const DX12ShaderResourceView&) = delete;

        void CreateResource(CD3DX12_HEAP_PROPERTIES prop, D3D12_RESOURCE_DESC desc) override;

        void CreateResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle) override;

    private:
        void UpdateSubresource();

    public:
        DX12ShaderResourceView(DX12Device* pDX12Device, DX12Command* pDX12Command, DX12HeapAllocator* pDX12HeapAllocator, D3D12_SHADER_RESOURCE_VIEW_DESC* shaderResourceViewDesc, D3D12_SUBRESOURCE_DATA* subData);
        ~DX12ShaderResourceView();

        void Initialize(CD3DX12_HEAP_PROPERTIES* heapProperties, D3D12_RESOURCE_DESC* resourceDesc, UINT heapIndex) override;

    public:
        ID3D12Resource* GetResouce() const { return m_pResource.Get(); }

        D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle() const { return m_descriptorHandle; }
    };
} // namespace

#endif // _DX12_SHADER_RESOURCE_VIEW_H_