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

    class DX12HeapAllocator
    {
    private:
        ComPtr<ID3D12DescriptorHeap>    m_descriptorHeap;

        D3D12_DESCRIPTOR_HEAP_DESC      m_descriptorHeapDesc;

        DX12Device*                     pDX12Device;

    private:
        DX12HeapAllocator(const DX12HeapAllocator&) = delete;
        DX12HeapAllocator& operator=(const DX12HeapAllocator) = delete;

    public:
        DX12HeapAllocator(DX12Device* pDX12Device, D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc);
        ~DX12HeapAllocator();

        bool CreateDescriptorHeap();

        D3D12_CPU_DESCRIPTOR_HANDLE GetStartDescriptorHeapHandle();
        D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHeapHandle(UINT heapIndex);
    };
} // namespace

#endif // _DX12_HEAP_ALLOCATOR_H_