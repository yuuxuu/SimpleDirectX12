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

#include "Param/BufferParam.h"

namespace Graphics
{
    class DX12ConstantBufferView : public IDX12Resouce
    {
    private:
        ComPtr<ID3D12Resource>              m_pResource;

        D3D12_CPU_DESCRIPTOR_HANDLE         m_CPUDescriptorHandle;
        
        D3D12_GPU_DESCRIPTOR_HANDLE         m_GPUDescriptorHandle;

        D3D12_CONSTANT_BUFFER_VIEW_DESC*    m_pConstantBufferViewDesc = nullptr;

        DX12Device*                         pDX12Device;

        DX12HeapAllocator*                  pDX12HeapAllocator;

        void*                               m_pMappedBuffer;

        Simple::BufferParam*                m_pParam;

    private:
        DX12ConstantBufferView(const DX12ConstantBufferView&) = delete;
        DX12ConstantBufferView& operator=(const DX12ConstantBufferView&) = delete;

        void CreateResource(CD3DX12_HEAP_PROPERTIES prop, D3D12_RESOURCE_DESC desc) override;

        void CreateResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle) override;

        void MapResouce();

    public:
        DX12ConstantBufferView(DX12Device* pDX12Device, DX12HeapAllocator* pDX12HeapAllocator, D3D12_CONSTANT_BUFFER_VIEW_DESC* constantBufferViewDesc, Simple::IParam* pParam);
        ~DX12ConstantBufferView();

        void Initialize(CD3DX12_HEAP_PROPERTIES* heapProperties, D3D12_RESOURCE_DESC* resourceDesc, UINT heapIndex) override;
        
        void UpdateResourceBuffer(const void* updateSource);

        ID3D12Resource* GetResource() const { return m_pResource.Get(); }

        D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle() const { return m_CPUDescriptorHandle; }
        
        D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle() const { return m_GPUDescriptorHandle; }
    };
} // namespace

#endif // _DX12_CONSTANT_BUFFER_VIEW_H_