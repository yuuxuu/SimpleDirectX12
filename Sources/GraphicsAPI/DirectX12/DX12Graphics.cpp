/**
 * @file DX12Graphics.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#include "DX12Graphics.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12Command.h"
#include "GraphicsAPI/DirectX12/DX12HeapAllocator.h"
#include "GraphicsAPI/DirectX12/DX12GraphicsResourceFactory.h"

#include "GraphicsAPI/DirectX12/DX12Resource/IDX12Resource.h"
#include "GraphicsAPI/DirectX12/DX12Resource/RTV/DX12RenderTargetView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/DSV/DX12DepthStencilView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/SRV/DX12ShaderResourceView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/CBV/DX12ConstantBurfferView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/VBV/DX12VertexBufferView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/IBV/DX12IndexBufferView.h"

#include "GraphicsAPI/Shader/DirectX/ShaderFactory.h"
#include "GraphicsAPI/Shader/DirectX/Shader.h"

#include "Texture/Texture.h"

namespace Graphics
{
    DX12Graphics::DX12Graphics() :
        m_windowHeight(0),
        m_windowWidth(0),
        m_primitiveTopology(0)
    {}

    DX12Graphics::~DX12Graphics()
    {}

    bool DX12Graphics::Initialize(HWND hwnd, UINT windowWidth, UINT windowHeight)
    {
        m_windowWidth = windowWidth;
        m_windowHeight = windowHeight;

        ComPtr<ID3D12CommandQueue> pDX12CommandQueue;

        m_pDX12Device = std::make_unique<DX12Device>();
        if (!m_pDX12Device->Initialize(pDX12CommandQueue, hwnd, windowWidth, windowHeight))
        {
            MessageBoxA(NULL, "DX12Device初期化：失敗", "MessageBox", MB_OK);
            return false;
        }

        m_pDX12Command = std::make_unique<DX12Command>(pDX12CommandQueue);
        if (!m_pDX12Command->Initialize(m_pDX12Device.get()))
        {
            MessageBoxA(NULL, "DX12Command初期化：失敗", "MessageBox", MB_OK);
            return false;
        }

        {
            const auto numRTV = 2;

            D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc =
            {
                D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
                numRTV,
                D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
            };

            auto pHeapAllocator = std::make_unique<DX12HeapAllocator>(m_pDX12Device.get(), descriptorHeapDesc);
            if (pHeapAllocator->CreateDescriptorHeap())
            {
                for (int i = 0; i < numRTV; i++)
                {
                    Simple::TextureDataParam param;
                    param.Width = windowWidth;
                    param.Height = windowHeight;

                    DX12RenderTargetViewResourceFactory resourceFactory;
                    auto renderTargetView = resourceFactory.CreateResource(m_pDX12Device.get(), pHeapAllocator.get(), &param);
                    if(renderTargetView)
                        pHeapAllocator->AddResource(renderTargetView);
                }

                m_pDX12HeapAllocatorMap[descriptorHeapDesc.Type] = std::move(pHeapAllocator);
            }
        }

        {
            const auto numDSV = 1;

            D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = 
            {
                D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
                numDSV,
                D3D12_DESCRIPTOR_HEAP_FLAG_NONE
            };

            auto pHeapAllocator = std::make_unique<DX12HeapAllocator>(m_pDX12Device.get(), descriptorHeapDesc);
            if (pHeapAllocator->CreateDescriptorHeap())
            {
                Simple::TextureDataParam param;
                param.Width = windowWidth;
                param.Height = windowHeight;

                DX12DepthStencilViewResourceFactory resourceFactory;
                auto depthStencilView = resourceFactory.CreateResource(m_pDX12Device.get(), pHeapAllocator.get(), &param);
                if (depthStencilView)
                    pHeapAllocator->AddResource(depthStencilView);

                m_pDX12HeapAllocatorMap[descriptorHeapDesc.Type] = std::move(pHeapAllocator);
            }
        }

        {
            const UINT numCBV_SRV_UAV = 1000;

            D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc =
            {
                D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
                numCBV_SRV_UAV,
                D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
            };

            auto pHeapAllocator = std::make_unique<DX12HeapAllocator>(m_pDX12Device.get(), descriptorHeapDesc);
            if (pHeapAllocator->CreateDescriptorHeap())
                m_pDX12HeapAllocatorMap[descriptorHeapDesc.Type] = std::move(pHeapAllocator);
        }

        InitializeGraphicsPipeline();

        return true;
    }

    DX12RenderTargetView* DX12Graphics::GetRenderTargetView(UINT index)
    {
        auto pHeapAllocatorItr = m_pDX12HeapAllocatorMap.find(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        if (pHeapAllocatorItr == m_pDX12HeapAllocatorMap.end())
        {
            MessageBoxA(NULL, "対象のHeapAllocatorが取得出来ませんでした。", "MessageBox", MB_OK);
            return nullptr;
        }

        auto pDX12Resource = pHeapAllocatorItr->second->GetDX12Resource(index);
        if (!pDX12Resource)
            return nullptr;

        return dynamic_cast<DX12RenderTargetView*>(pDX12Resource);
    }

    DX12DepthStencilView* DX12Graphics::GetDepthStencilView()
    {
        auto pHeapAllocatorItr = m_pDX12HeapAllocatorMap.find(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        if (pHeapAllocatorItr == m_pDX12HeapAllocatorMap.end())
        {
            MessageBoxA(NULL, "対象のHeapAllocatorが取得出来ませんでした。", "MessageBox", MB_OK);
            return nullptr;
        }

        auto pDX12Resource = pHeapAllocatorItr->second->GetDX12Resource(0);
        if (!pDX12Resource)
            return nullptr;

        return dynamic_cast<DX12DepthStencilView*>(pDX12Resource);
    }

    void DX12Graphics::RenderPrepare()
    {
        if (m_windowWidth == 0 && m_windowHeight == 0)
        {
            MessageBoxA(NULL, "windowの幅と高さが0です。", "MessageBox", MB_OK);
            return;
        }

        auto index = m_pDX12Device->GetCurrentBackBufferIndex();

        auto pRenderTargetView = GetRenderTargetView(index);
        if (!pRenderTargetView)
        {
            MessageBoxA(NULL, "DX12RenderTargetViewのキャストに失敗しました。", "MessageBox", MB_OK);
            return;
        }
        auto renderTargetViewHandle = pRenderTargetView->GetCPUDescriptorHandle();

        auto pDepthStencilView = GetDepthStencilView();
        if (!pDepthStencilView)
        {
            MessageBoxA(NULL, "DX12DepthStencilViewのキャストに失敗しました。", "MessageBox", MB_OK);
            return;
        }
        auto depthStencilViewHandle = pDepthStencilView->GetCPUDescriptorHandle();

        m_pDX12Command->ResetCommandList();

        m_pDX12Command->SetRect(m_windowWidth, m_windowHeight);
        m_pDX12Command->SetViewPort(0, 0, m_windowWidth, m_windowHeight);

        m_pDX12Command->SetResourceBarrier(pRenderTargetView->GetResource(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        m_pDX12Command->SetResourceBarrier(pDepthStencilView->GetResource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

        m_pDX12Command->ClearRenderTargetView(renderTargetViewHandle);
        m_pDX12Command->ClearDepthStencilView(depthStencilViewHandle, D3D12_CLEAR_FLAG_DEPTH);
        m_pDX12Command->SetRenderTargetView(&renderTargetViewHandle, &depthStencilViewHandle);

        SetGraphicsPipeline();
    }

    void DX12Graphics::RenderEnd()
    {
        auto index = m_pDX12Device->GetCurrentBackBufferIndex();

        auto pRenderTargetView = GetRenderTargetView(index);
        if (!pRenderTargetView)
        {
            MessageBoxA(NULL, "DX12RenderTargetViewのキャストに失敗しました。", "MessageBox", MB_OK);
            return;
        }
        auto renderTargetViewHandle = pRenderTargetView->GetCPUDescriptorHandle();

        auto pDepthStencilView = GetDepthStencilView();
        if (!pDepthStencilView)
        {
            MessageBoxA(NULL, "DX12DepthStencilViewのキャストに失敗しました。", "MessageBox", MB_OK);
            return;
        }
        auto depthStencilViewHandle = pDepthStencilView->GetCPUDescriptorHandle();

        m_pDX12Command->SetResourceBarrier(pRenderTargetView->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        m_pDX12Command->SetResourceBarrier(pDepthStencilView->GetResource(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);

        m_pDX12Command->ExecuteComandLists();

        m_pDX12Device->Present();
    }

    void DX12Graphics::InitializeGraphicsPipeline()
    {
        Shader::ModelShaderFactory shaderFactory;
        auto pShader = shaderFactory.CreateShader(m_pDX12Device.get(), m_pDX12Command.get());
        if(pShader)
            m_pShaderVec.emplace_back(std::move(pShader));
    }

    void DX12Graphics::SetGraphicsPipeline()
    {
        auto pShader = m_pShaderVec.front().get();
        if (!pShader)
            return;

        UINT primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        pShader->SetGraphicsRootSignature();
        pShader->SetPiplineState(primitiveTopologyType);

        switch (primitiveTopologyType)
        {
            case D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE:
                m_primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                break;
            case D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE:
                m_primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
                break;
            case D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT:
                m_primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
                break;
        }

        auto pHeapAllocatorItr = m_pDX12HeapAllocatorMap.find(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        if (pHeapAllocatorItr == m_pDX12HeapAllocatorMap.end())
        {
            MessageBoxA(NULL, "対象のHeapAllocatorが取得出来ませんでした。", "MessageBox", MB_OK);
            return;
        }

        pHeapAllocatorItr->second->SetDescriptorHeap(m_pDX12Command.get());
    }

    void DX12Graphics::InitializeGraphicsBufferResource(IGraphicsResource*& pGraphicsResource, Simple::IParam* pParam, GraphicsResourceType graphicsResourceType)
    {
        auto pHeapAllocatorItr = m_pDX12HeapAllocatorMap.find(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        if (pHeapAllocatorItr == m_pDX12HeapAllocatorMap.end())
        {
            MessageBoxA(NULL, "対象のHeapAllocatorが取得出来ませんでした。\nGraphicsResourceの初期化が出来ませんでした。", "MessageBox", MB_OK);
            return;
        }
         
        std::unique_ptr<IGraphicsResourceFactory> pResourceFactory = nullptr;

        switch (graphicsResourceType)
        {
        case GraphicsResourceType::SRV:
            pResourceFactory = std::make_unique<DX12ShaderResourceViewResourceFactory>(m_pDX12Command.get());
            break;
        case GraphicsResourceType::CBV:
            pResourceFactory = std::make_unique<DX12ConstantBufferViewResourceFactory>();
            break;
        case GraphicsResourceType::VBV:
            pResourceFactory = std::make_unique<DX12VertexBufferResourceFactory>();
            break;
        case GraphicsResourceType::IBV:
            pResourceFactory = std::make_unique<DX12IndexBufferResourceFactory>();
            break;
        default:
            MessageBoxA(NULL, "対象のResourceBufferView作成処理がありません。", "MessageBox", MB_OK);
            return;
        }

        if (!pResourceFactory)
            return;

        auto pDX12Resource = pResourceFactory->CreateResource(m_pDX12Device.get(), pHeapAllocatorItr->second.get(), pParam);
        if (!pDX12Resource)
            return;

        pGraphicsResource = pDX12Resource.get();

        pHeapAllocatorItr->second->AddResource(pDX12Resource);
    }

    void DX12Graphics::UpdateGraphicsBufferResource(
        IGraphicsResource* pGraphicsResource, 
        const void* updateSource, 
        Simple::IParam* pParam,
        GraphicsResourceType graphicsResourceType)
    {
        switch (graphicsResourceType)
        {
        case GraphicsResourceType::CBV:
        {
            auto pDX12ConstantBufferView = dynamic_cast<DX12ConstantBufferView*>(pGraphicsResource);
            if (pDX12ConstantBufferView == nullptr)
            {
                MessageBoxA(NULL, "DX12ConstantBufferViewのキャストに失敗しました。", "MessageBox", MB_OK);
                return;
            }

            pDX12ConstantBufferView->UpdateResourceBuffer(updateSource, pParam);
        }
        break;
        case GraphicsResourceType::VBV:
        {
            auto pDX12VertexBufferView = dynamic_cast<DX12VertexBufferView*>(pGraphicsResource);
            if (pDX12VertexBufferView == nullptr)
            {
                MessageBoxA(NULL, "DX12ConstantBufferViewのキャストに失敗しました。", "MessageBox", MB_OK);
                return;
            }

            pDX12VertexBufferView->UpdateResourceBuffer(updateSource, pParam);
        }
        break;
        case GraphicsResourceType::IBV:
        {
            auto pDX12IndexBufferView = dynamic_cast<DX12IndexBufferView*>(pGraphicsResource);
            if (pDX12IndexBufferView == nullptr)
            {
                MessageBoxA(NULL, "DX12ConstantBufferViewのキャストに失敗しました。", "MessageBox", MB_OK);
                return;
            }

            pDX12IndexBufferView->UpdateResourceBuffer(updateSource, pParam);
        }
        break;
        }
    }

    void DX12Graphics::SetConstantBufferView(UINT index, IGraphicsResource* pGraphicsResource)
    {
        auto pDX12ConstantBufferView = dynamic_cast<DX12ConstantBufferView*>(pGraphicsResource);
        if (pDX12ConstantBufferView == nullptr)
        {
            MessageBoxA(NULL, "DX12ConstantBufferViewのキャストに失敗しました。", "MessageBox", MB_OK);
            return;
        }

        m_pDX12Command->SetGraphicsRootDescriptorTable(index, pDX12ConstantBufferView->GetGPUDescriptorHandle());
    }

    void DX12Graphics::SetShaderResourceView(UINT index, IGraphicsResource* pGraphicsResource)
    {
        auto pDX12ShaderResourceView = dynamic_cast<DX12ShaderResourceView*>(pGraphicsResource);
        if (pDX12ShaderResourceView == nullptr)
        {
            MessageBoxA(NULL, "DX12ShaderResourceViewのキャストに失敗しました。", "MessageBox", MB_OK);
            return;
        }

        m_pDX12Command->SetGraphicsRootDescriptorTable(index, pDX12ShaderResourceView->GetGPUDescriptorHandle());
    }

    void DX12Graphics::DrawInstancedVertexBuffer(IGraphicsResource* pVertexResource, UINT numVerties)
    {
        auto pDX12VertexBufferView = dynamic_cast<DX12VertexBufferView*>(pVertexResource);
        if (pDX12VertexBufferView == nullptr)
        {
            MessageBoxA(NULL, "DX12VertexBufferViewのキャストに失敗しました。", "MessageBox", MB_OK);
            return;
        }

        auto pVertexBufferView = pDX12VertexBufferView->GetVertexBufferView();
        m_pDX12Command->SetBuffer(pVertexBufferView, nullptr);

        m_pDX12Command->DrawInstanced(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(m_primitiveTopology), numVerties);
    }

    void DX12Graphics::DrawIndexedIndexBuffer(IGraphicsResource* pVertexResource, IGraphicsResource* pIndexResource, UINT numIndies)
    {
        auto pDX12VertexBufferView = dynamic_cast<DX12VertexBufferView*>(pVertexResource);
        if (pDX12VertexBufferView == nullptr)
        {
            MessageBoxA(NULL, "DX12VertexBufferViewのキャストに失敗しました。", "MessageBox", MB_OK);
            return;
        }

        auto pVertexBufferView = pDX12VertexBufferView->GetVertexBufferView();
        D3D12_INDEX_BUFFER_VIEW* pIndexBufferview = nullptr;

        auto pDX12IndexBufferView = dynamic_cast<DX12IndexBufferView*>(pIndexResource);
        if (pDX12IndexBufferView != nullptr)
            pIndexBufferview = pDX12IndexBufferView->GetIndexBufferView();

        m_pDX12Command->SetBuffer(pVertexBufferView, pIndexBufferview);

        m_pDX12Command->DrawIndexed(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(m_primitiveTopology), numIndies);
    }

} // namespace