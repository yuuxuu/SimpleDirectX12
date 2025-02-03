/**
 * @file DX12RenderTargetView.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#pragma once

#ifndef _DX12_RENDER_TARGET_VIEW_H_
#define _DX12_RENDER_TARGET_VIEW_H_

#include "GraphicsAPI/DirectX12/DX12Resource/IDX12Resource.h"

namespace Graphics
{
    class DX12RenderTargetView : public IDX12Resouce
    {
    private:
        ComPtr<ID3D12Resource>          m_pResource;

        D3D12_CPU_DESCRIPTOR_HANDLE     m_CPUDescriptorHandle;

        D3D12_GPU_DESCRIPTOR_HANDLE     m_GPUDescriptorHandle;

        D3D12_RENDER_TARGET_VIEW_DESC*  m_pRenderTargetViewDesc = nullptr;

        DX12Device*                     pDX12Device;

        DX12HeapAllocator*              pDX12HeapAllocator;

    private:
        DX12RenderTargetView(const DX12RenderTargetView&) = delete;
        DX12RenderTargetView& operator=(const DX12RenderTargetView&) = delete;

        void CreateResource(CD3DX12_HEAP_PROPERTIES prop, D3D12_RESOURCE_DESC desc) override;

        void CreateResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle) override;

    public:
        explicit DX12RenderTargetView(DX12Device* pDX12Device, DX12HeapAllocator* pDX12HeapAllocator, D3D12_RENDER_TARGET_VIEW_DESC* renderTargetViewDesc);
        ~DX12RenderTargetView();

        void Initialize(CD3DX12_HEAP_PROPERTIES* heapProperties, D3D12_RESOURCE_DESC* resourceDesc, UINT heapIndex) override;

        ID3D12Resource* GetResource() const { return m_pResource.Get(); }

        D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle() const { return m_CPUDescriptorHandle; }
        
        D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle() const { return m_GPUDescriptorHandle; }
    };
} // namespace

#endif // _DX12_RENDER_TARGET_VIEW_H_