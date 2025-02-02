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

namespace Graphics
{
    class DX12Device;
    class DX12Command;

    class DX12HeapAllocator
    {
    private:
        ComPtr<ID3D12DescriptorHeap>    m_descriptorHeap;

        D3D12_DESCRIPTOR_HEAP_DESC      m_descriptorHeapDesc;

        DX12Device*                     pDX12Device;

        UINT                            m_heapIndex;

    private:
        DX12HeapAllocator(const DX12HeapAllocator&) = delete;
        DX12HeapAllocator& operator=(const DX12HeapAllocator) = delete;

    public:
        DX12HeapAllocator(DX12Device* pDX12Device, D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc);
        ~DX12HeapAllocator();

        bool CreateDescriptorHeap();

        void SetDescriptorHeap(DX12Command* pDX12Command);

        D3D12_CPU_DESCRIPTOR_HANDLE GetStartCPUDescriptorHeapHandle();
        D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHeapHandle(UINT heapIndex);

        D3D12_GPU_DESCRIPTOR_HANDLE GetStartGPUDescriptorHeapHandle();
        D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHeapHandle(UINT heapIndex);

        void AddHeapIndex();

        const UINT GetHeapIndex() const { return m_heapIndex; }

        const UINT GetMaxNumDescriptors() const { return m_descriptorHeapDesc.NumDescriptors; }

        const bool IsVisibleShader() const { return m_descriptorHeapDesc.Flags == D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; }
    };
} // namespace

#endif // _DX12_HEAP_ALLOCATOR_H_