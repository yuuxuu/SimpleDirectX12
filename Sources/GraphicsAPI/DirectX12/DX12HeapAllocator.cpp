/**
 * @file DX12HeapAllocator.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#include "DX12HeapAllocator.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"

namespace Graphics
{
    DX12HeapAllocator::DX12HeapAllocator(DX12Device* pDX12Device, D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc) :
        pDX12Device(pDX12Device),
        m_descriptorHeapDesc(descriptorHeapDesc),
        m_heapIndex(0)
    {}

    DX12HeapAllocator::~DX12HeapAllocator()
    {}

    bool DX12HeapAllocator::CreateDescriptorHeap()
    {
        if (!pDX12Device->CreateDescriptorHeap(m_descriptorHeap, m_descriptorHeapDesc))
        {
            MessageBoxA(NULL, "DescriptorHeap作成：失敗", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DX12HeapAllocator::GetStartCPUDescriptorHeapHandle()
    {
        return m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DX12HeapAllocator::GetCPUDescriptorHeapHandle(UINT heapIndex)
    {
        if (heapIndex == 0)
            return GetStartCPUDescriptorHeapHandle();
        else if (heapIndex >= m_descriptorHeapDesc.NumDescriptors)
            heapIndex = m_descriptorHeapDesc.NumDescriptors - 1;

        auto handle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
        auto handleSize = pDX12Device->GetDescriptorHandleIncrementSize(m_descriptorHeapDesc.Type);

        handle.ptr += (handleSize * heapIndex);

        return handle;
    }

    void DX12HeapAllocator::AddHeapIndex()
    {
        if (m_heapIndex >= m_descriptorHeapDesc.NumDescriptors)
        {
            MessageBoxA(NULL, "Descriptor最大値を超えています。", "MessageBox", MB_OK);
            return;
        }

        m_heapIndex++;
    }

} // namespace