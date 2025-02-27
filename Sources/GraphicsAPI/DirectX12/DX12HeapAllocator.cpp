/**
 * @file DX12HeapAllocator.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#include "DX12HeapAllocator.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12Command.h"

namespace Graphics
{
    DX12HeapAllocator::DX12HeapAllocator(DX12Device* pDX12Device, D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc) :
        pDX12Device(pDX12Device),
        m_descriptorHeapDesc(descriptorHeapDesc)
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

    void DX12HeapAllocator::SetDescriptorHeap(DX12Command* pDX12Command)
    {
        pDX12Command->SetDescriptorHeaps(m_descriptorHeap.Get());
    }

    void DX12HeapAllocator::AddResource(std::unique_ptr<IDX12Resouce>& pDX12Resource)
    {
        m_DX12ResourceVec.emplace_back(std::move(pDX12Resource));
    }

    IDX12Resouce* DX12HeapAllocator::GetDX12Resource(UINT index)
    {
        auto& pDX12Resource = m_DX12ResourceVec.at(index);
        if (!pDX12Resource)
            return nullptr;

        return pDX12Resource.get();
    }

    const D3D12_CPU_DESCRIPTOR_HANDLE DX12HeapAllocator::GetStartCPUDescriptorHeapHandle() const
    {
        return m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    }

    const D3D12_CPU_DESCRIPTOR_HANDLE DX12HeapAllocator::GetCPUDescriptorHeapHandle(UINT heapIndex) const
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

    const D3D12_GPU_DESCRIPTOR_HANDLE DX12HeapAllocator::GetStartGPUDescriptorHeapHandle() const
    {
        return m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
    }

    const D3D12_GPU_DESCRIPTOR_HANDLE DX12HeapAllocator::GetGPUDescriptorHeapHandle(UINT heapIndex) const
    {
        if (heapIndex == 0)
            return GetStartGPUDescriptorHeapHandle();
        else if (heapIndex >= m_descriptorHeapDesc.NumDescriptors)
            heapIndex = m_descriptorHeapDesc.NumDescriptors - 1;

        auto handle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
        auto handleSize = pDX12Device->GetDescriptorHandleIncrementSize(m_descriptorHeapDesc.Type);

        handle.ptr += (handleSize * heapIndex);

        return handle;
    }

    const UINT DX12HeapAllocator::GetHeapIndex() const
    {
        return static_cast<UINT>(m_DX12ResourceVec.size());
    }

    const UINT DX12HeapAllocator::GetMaxNumDescriptors() const
    {
        return m_descriptorHeapDesc.NumDescriptors;
    }

    const bool DX12HeapAllocator::IsVisibleShader() const
    {
        return m_descriptorHeapDesc.Flags == D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    }

} // namespace