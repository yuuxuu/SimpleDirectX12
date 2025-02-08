/**
 * @file DX12GraphicsResourceBuilder.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#include "DX12GraphicsResourceBuilder.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12Command.h"
#include "GraphicsAPI/DirectX12/DX12HeapAllocator.h"

#include "GraphicsAPI/DirectX12/DX12Resource/RTV/DX12RenderTargetView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/DSV/DX12DepthStencilView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/CBV/DX12ConstantBurfferView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/VBV/DX12VertexBufferView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/IBV/DX12IndexBufferView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/SRV/DX12ShaderResourceView.h"

#include "Param/TextureDataParam.h"
#include "Param/BufferParam.h"

namespace Graphics
{
    DX12GraphicsResourceBuilder::DX12GraphicsResourceBuilder()
    {}

    DX12GraphicsResourceBuilder::~DX12GraphicsResourceBuilder()
    {}

    void DX12GraphicsResourceBuilder::CreateRenderTargetView(
        DX12Device* pDX12Device, 
        DX12HeapAllocator* pHeapAllocator, 
        Simple::IParam* pParam,
        std::unique_ptr<IDX12Resouce>& pDX12Resource)
    {
        auto renderTargetView = std::make_unique<DX12RenderTargetView>(pDX12Device, pHeapAllocator, nullptr);
        renderTargetView->Initialize(nullptr, nullptr, pHeapAllocator->GetHeapIndex());

        pDX12Resource = std::move(renderTargetView);
    }

    void DX12GraphicsResourceBuilder::CreateDepthStencilView(
        DX12Device* pDX12Device, 
        DX12HeapAllocator* pHeapAllocator,
        Simple::IParam* pParam,
        std::unique_ptr<IDX12Resouce>& pDX12Resource)
    {
        auto pTextureDataParam = dynamic_cast<Simple::TextureDataParam*>(pParam);
        if (!pTextureDataParam)
        {
            MessageBoxA(NULL, "TextureDataParamへのキャストに失敗しました。", "MessageBox", MB_OK);
            return;
        }

        D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilviewDesc = {};

        depthStencilviewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilviewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        depthStencilviewDesc.Flags = D3D12_DSV_FLAG_NONE;
        depthStencilviewDesc.Texture2D.MipSlice = 0;

        D3D12_RESOURCE_DESC resourceDesc = {};

        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        resourceDesc.Width = pTextureDataParam->Width;
        resourceDesc.Height = pTextureDataParam->Height;
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
        resourceDesc.SampleDesc = { 1 , 0 };
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

        auto pDepthStencilView = std::make_unique<DX12DepthStencilView>(pDX12Device,pHeapAllocator, &depthStencilviewDesc);
        pDepthStencilView->Initialize(&prop, &resourceDesc);

        pDX12Resource = std::move(pDepthStencilView);
    }

    void DX12GraphicsResourceBuilder::CreateConstantBufferView(
        DX12Device* pDX12Device,
        DX12HeapAllocator* pHeapAllocator,
        Simple::IParam* pParam,
        std::unique_ptr<IDX12Resouce>& pDX12Resource)
    {
        auto pBufferParam = dynamic_cast<Simple::BufferParam*>(pParam);
        if (!pBufferParam)
        {
            MessageBoxA(NULL, "BufferParamへのキャストに失敗しました。", "MessageBox", MB_OK);
            return;
        }

        D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferViewDesc = {};

        constantBufferViewDesc.SizeInBytes = (pBufferParam->byteWidth + 0xff) & ~0xff;

        D3D12_RESOURCE_DESC resourceDesc = {};

        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resourceDesc.Width = constantBufferViewDesc.SizeInBytes;
        resourceDesc.Height = 1;
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        resourceDesc.SampleDesc.Count = 1;

        auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        auto pConstantBufferView = std::make_unique<DX12ConstantBufferView>(pDX12Device, pHeapAllocator, &constantBufferViewDesc);
        pConstantBufferView->Initialize(&prop, &resourceDesc, pHeapAllocator->GetHeapIndex());

        pHeapAllocator->AddHeapIndex();

        pDX12Resource = std::move(pConstantBufferView);
    }

    void DX12GraphicsResourceBuilder::CreateVertexBufferView(
        DX12Device* pDX12Device,
        DX12HeapAllocator* pHeapAllocator,
        Simple::IParam* pParam,
        std::unique_ptr<IDX12Resouce>& pDX12Resource)
    {
        auto pBufferParam = dynamic_cast<Simple::BufferParam*>(pParam);
        if (!pBufferParam)
        {
            MessageBoxA(NULL, "BufferParamへのキャストに失敗しました。", "MessageBox", MB_OK);
            return;
        }

        D3D12_RESOURCE_DESC resourceDesc = {};

        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resourceDesc.Width = pBufferParam->byteWidth;
        resourceDesc.Height = 1;
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        resourceDesc.SampleDesc.Count = 1;

        auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        auto pVertexBufferView = std::make_unique<DX12VertexBufferView>(pDX12Device, pBufferParam);
        pVertexBufferView->Initialize(&prop, &resourceDesc);

        pDX12Resource = std::move(pVertexBufferView);
    }

    void DX12GraphicsResourceBuilder::CreateIndexBufferView(
        DX12Device* pDX12Device,
        DX12HeapAllocator* pHeapAllocator,
        Simple::IParam* pParam,
        std::unique_ptr<IDX12Resouce>& pDX12Resource)
    {
        auto pBufferParam = dynamic_cast<Simple::BufferParam*>(pParam);
        if (!pBufferParam)
        {
            MessageBoxA(NULL, "BufferParamへのキャストに失敗しました。", "MessageBox", MB_OK);
            return;
        }

        D3D12_RESOURCE_DESC resourceDesc = {};

        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resourceDesc.Width = pBufferParam->byteWidth;
        resourceDesc.Height = 1;
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        resourceDesc.SampleDesc.Count = 1;

        auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        auto pIndexBufferView = std::make_unique<DX12IndexBufferView>(pDX12Device, pBufferParam);
        pIndexBufferView->Initialize(&prop, &resourceDesc);

        pDX12Resource = std::move(pIndexBufferView);
    }

    void DX12GraphicsResourceBuilder::CreateShaderResourceView(
        DX12Device* pDX12Device, 
        DX12Command* pDX12Command,
        DX12HeapAllocator* pHeapAllocator,
        Simple::IParam* pParam,
        std::unique_ptr<IDX12Resouce>& pGraphicsResource)
    {
        auto pTextureDataParam = dynamic_cast<Simple::TextureDataParam*>(pParam);
        if (!pTextureDataParam)
        {
            MessageBoxA(NULL, "TextureDataParamへのキャストに失敗しました。", "MessageBox", MB_OK);
            return;
        }

        D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};

        shaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        shaderResourceViewDesc.Format = pTextureDataParam->Format;
        shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        shaderResourceViewDesc.Texture2D.MipLevels = 1;

        D3D12_RESOURCE_DESC resourceDesc = {};

        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        resourceDesc.Width = pTextureDataParam->Width;
        resourceDesc.Height = pTextureDataParam->Height;
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.Format = pTextureDataParam->Format;
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        resourceDesc.SampleDesc.Count = 1;

        auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

        D3D12_SUBRESOURCE_DATA subData = {};

        subData.pData = pTextureDataParam->pData.get();
        subData.RowPitch = pTextureDataParam->RowPitch;
        subData.SlicePitch = pTextureDataParam->SlicePitch;

        auto pShaderResourceView = std::make_unique<DX12ShaderResourceView>(pDX12Device, pDX12Command, pHeapAllocator, &shaderResourceViewDesc, &subData);
        pShaderResourceView->Initialize(&prop, &resourceDesc, pHeapAllocator->GetHeapIndex());

        pHeapAllocator->AddHeapIndex();

        pGraphicsResource = std::move(pShaderResourceView);
    }

} // namespace