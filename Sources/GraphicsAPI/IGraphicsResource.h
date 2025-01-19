/**
 * @file IGraphicsResource.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#ifndef _IGRAPHICS_RESOURCE_H_
#define _IGRAPHICS_RESOURCE_H_

namespace Graphics
{
    class IGraphicsResource
    {
    protected:
        IGraphicsResource() {}
        virtual ~IGraphicsResource() {}

        virtual void CreateResource(CD3DX12_HEAP_PROPERTIES prop, D3D12_RESOURCE_DESC desc) = 0;

        virtual void CreateResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle) = 0;

        virtual void Initialize(CD3DX12_HEAP_PROPERTIES* prop, D3D12_RESOURCE_DESC* resourceDesc, UINT heapIndex = 0) = 0;
    };
} // namespace

#endif // _IGRAPHICS_RESOURCE_H_

