/**
 * @file IDX12Resource.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#pragma once

#ifndef _IDX12_RESOURCE_H_
#define _IDX12_RESOURCE_H_

#include "GraphicsAPI/IGraphicsResource.h"

using Microsoft::WRL::ComPtr;

namespace Graphics
{
    class DX12Device;
    class DX12Command;
    class DX12HeapAllocator;

    class IDX12Resouce : public IGraphicsResource
    {
    public:
        IDX12Resouce() {}
        virtual ~IDX12Resouce() {}

        virtual void Initialize(CD3DX12_HEAP_PROPERTIES* prop, D3D12_RESOURCE_DESC* resourceDesc, UINT heapIndex = 0) = 0;
        
        virtual void CreateResource(CD3DX12_HEAP_PROPERTIES prop, D3D12_RESOURCE_DESC desc) = 0;

        virtual void CreateResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle) = 0;
    };

} // namespace

#endif // _IDX12_RESOURCE_H_