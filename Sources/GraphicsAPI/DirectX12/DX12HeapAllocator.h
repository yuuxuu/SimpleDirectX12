/**
 * @file DX12HeapAllocator.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#ifndef _DX12_HEAP_ALLOCATOR_H_
#define _DX12_HEAP_ALLOCATOR_H_

using Microsoft::WRL::ComPtr;

#include "GraphicsAPI/DirectX12/DX12Resource/IDX12Resource.h"

namespace Graphics
{
    class DX12Device;
    class DX12Command;

    using DX12ResourcVec = std::vector<std::unique_ptr<IDX12Resouce>>;

    class DX12HeapAllocator
    {
    private:
        ComPtr<ID3D12DescriptorHeap>    m_descriptorHeap;

        D3D12_DESCRIPTOR_HEAP_DESC      m_descriptorHeapDesc;

        DX12Device*                     pDX12Device;

        DX12ResourcVec                  m_DX12ResourceVec;

    private:
        DX12HeapAllocator(const DX12HeapAllocator&) = delete;
        DX12HeapAllocator& operator=(const DX12HeapAllocator) = delete;

    public:
        explicit DX12HeapAllocator(DX12Device* pDX12Device, D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc);
        ~DX12HeapAllocator();

        bool CreateDescriptorHeap();

        void SetDescriptorHeap(DX12Command* pDX12Command);

        void AddResource(std::unique_ptr<IDX12Resouce>& pDX12Resource);

        IDX12Resouce* GetDX12Resource(UINT index);

        const D3D12_CPU_DESCRIPTOR_HANDLE GetStartCPUDescriptorHeapHandle() const;
        const D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHeapHandle(UINT heapIndex) const;

        const D3D12_GPU_DESCRIPTOR_HANDLE GetStartGPUDescriptorHeapHandle() const;
        const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHeapHandle(UINT heapIndex) const;

        const UINT GetHeapIndex() const;

        const UINT GetMaxNumDescriptors() const;

        const bool IsVisibleShader() const;
    };
} // namespace

#endif // _DX12_HEAP_ALLOCATOR_H_