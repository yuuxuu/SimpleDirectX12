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
#include "GraphicsAPI/DirectX12/DX12GraphicsResourceBuilder.h"

#include "GraphicsAPI/DirectX12/DX12Resource/RTV/DX12RenderTargetView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/DSV/DX12DepthStencilView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/SRV/DX12ShaderResourceView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/CBV/DX12ConstantBurfferView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/VBV/DX12VertexBufferView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/IBV/DX12IndexBufferView.h"

#include "GraphicsAPI/Shader/DirectX/Shader.h"

#include "Texture/Texture.h"

namespace Graphics
{
    DX12Graphics::DX12Graphics()
    {}

    DX12Graphics::~DX12Graphics()
    {}

    bool DX12Graphics::Initialize(HWND hwnd, UINT windowWidth, UINT windowHeight)
    {
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

            auto heapAllocator = std::make_unique<DX12HeapAllocator>(m_pDX12Device.get(), descriptorHeapDesc);
            if (heapAllocator->CreateDescriptorHeap())
            {
                for (int i = 0; i < numRTV; i++)
                {
                    std::unique_ptr<IDX12Resouce> renderTargetView;

                    Simple::TextureDataParam param;
                    param.Width = windowWidth;
                    param.Height = windowHeight;

                    DX12GraphicsResourceBuilder::CreateRenderTargetView(
                        m_pDX12Device.get(),
                        heapAllocator.get(),
                        &param,
                        renderTargetView);

                    heapAllocator->AddHeapIndex();

                    m_pRenderTargetViews.push_back(std::move(renderTargetView));
                }

                m_pDX12HeapAllocatorMap[descriptorHeapDesc.Type] = std::move(heapAllocator);
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

            auto heapAllocator = std::make_unique<DX12HeapAllocator>(m_pDX12Device.get(), descriptorHeapDesc);
            if (heapAllocator->CreateDescriptorHeap())
            {
                Simple::TextureDataParam param;
                param.Width = windowWidth;
                param.Height = windowHeight;

                DX12GraphicsResourceBuilder::CreateDepthStencilView(
                    m_pDX12Device.get(),
                    heapAllocator.get(),
                    &param,
                    m_pDepthStencilView);

                m_pDX12HeapAllocatorMap[descriptorHeapDesc.Type] = std::move(heapAllocator);
            }
        }

        {
            const UINT numCBV_SRV_UAV = 100;

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

    void DX12Graphics::Finalize()
    {
        auto index = m_pDX12Device->GetCurrentBackBufferIndex();

        auto renderTargetView = dynamic_cast<DX12RenderTargetView*>(m_pRenderTargetViews.at(index).get());
        auto renderTargetViewHandle = renderTargetView->GetCPUDescriptorHandle();
        auto depthStencilView = dynamic_cast<DX12DepthStencilView*>(m_pDepthStencilView.get());
        auto depthStencilViewHandle = depthStencilView->GetCPUDescriptorHandle();

        m_pDX12Command->SetResourceBarrier(renderTargetView->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        m_pDX12Command->SetResourceBarrier(depthStencilView->GetResource(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);

        m_pDX12Command->ExecuteComandLists();

        m_pDX12Device->Present();
    }

    void DX12Graphics::Update(const UINT windowWidth, const UINT windowHeight)
    {
        auto index = m_pDX12Device->GetCurrentBackBufferIndex();

        auto renderTargetView = dynamic_cast<DX12RenderTargetView*>(m_pRenderTargetViews.at(index).get());
        auto renderTargetViewHandle = renderTargetView->GetCPUDescriptorHandle();
        auto depthStencilView = dynamic_cast<DX12DepthStencilView*>(m_pDepthStencilView.get());
        auto depthStencilViewHandle = depthStencilView->GetCPUDescriptorHandle();

        m_pDX12Command->ResetCommandList();

        m_pDX12Command->SetRect(windowWidth, windowHeight);
        m_pDX12Command->SetViewPort(0, 0, windowWidth, windowHeight);

        m_pDX12Command->SetResourceBarrier(renderTargetView->GetResource(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        m_pDX12Command->SetResourceBarrier(depthStencilView->GetResource(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

        m_pDX12Command->ClearRenderTargetView(renderTargetViewHandle);
        m_pDX12Command->ClearDepthStencilView(depthStencilViewHandle, D3D12_CLEAR_FLAG_DEPTH);
        m_pDX12Command->SetRenderTargetView(&renderTargetViewHandle, &depthStencilViewHandle);

        SetGraphicsPipeline();
    }

    void DX12Graphics::InitializeGraphicsPipeline()
    {
        auto itr = std::filesystem::recursive_directory_iterator("Resources/Shader/HLSL");
        for (const auto& file : itr)
        {
            auto extension = file.path().extension();
            if (extension != ".hlsl")
                continue;

            auto pShader = std::make_unique<Shader::Shader>(m_pDX12Device.get(), m_pDX12Command.get());
            if (!pShader->Initialize(file.path().generic_string().c_str()))
                continue;

            D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc = {};

            pShader->SetGraphicsPipelineState(graphicsPipelineStateDesc);

            auto pPipeline = std::make_unique<Shader::GraphicsPipeline>(m_pDX12Device.get(), m_pDX12Command.get());
            if (pPipeline->InitializePipeline(graphicsPipelineStateDesc))
            {
                m_pShaderVec.push_back(std::move(pShader));

                m_pGraphicsPipelineVec.push_back(std::move(pPipeline));
            }
        }
    }

    void DX12Graphics::SetGraphicsPipeline()
    {
        auto pPipeline = m_pGraphicsPipelineVec.front().get();
        if (!pPipeline)
            return;

        auto pShader = m_pShaderVec.front().get();
        if (!pShader)
            return;

        pShader->SetRootSignature();

        pPipeline->SetPipeline();

        auto pHeapAllocatorItr = m_pDX12HeapAllocatorMap.find(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        if (pHeapAllocatorItr == m_pDX12HeapAllocatorMap.end())
        {
            MessageBoxA(NULL, "対象のHeapAllocatorが取得出来ませんでした。\nGraphicsResourceの初期化が出来ませんでした。", "MessageBox", MB_OK);
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
         
        std::unique_ptr<IDX12Resouce> pDX12Resource;

        switch (graphicsResourceType)
        {
        case GraphicsResourceType::SRV:
            DX12GraphicsResourceBuilder::CreateShaderResourceView(m_pDX12Device.get(), m_pDX12Command.get(), pHeapAllocatorItr->second.get(), pParam, pDX12Resource);
            break;
        case GraphicsResourceType::CBV:
            DX12GraphicsResourceBuilder::CreateConstantBufferView(m_pDX12Device.get(), pHeapAllocatorItr->second.get(), pParam, pDX12Resource);
            break;
        case GraphicsResourceType::VBV:
            DX12GraphicsResourceBuilder::CreateVertexBufferView(m_pDX12Device.get(), pHeapAllocatorItr->second.get(), pParam, pDX12Resource);
            break;
        case GraphicsResourceType::IBV:
            DX12GraphicsResourceBuilder::CreateIndexBufferView(m_pDX12Device.get(), pHeapAllocatorItr->second.get(), pParam, pDX12Resource);
            break;
        default:
            MessageBoxA(NULL, "対象のResourceBufferView作成処理がありません。", "MessageBox", MB_OK);
            return;
        }

        pGraphicsResource = pDX12Resource.get();

        m_pGraphicsBufferResourceViews.push_back(std::move(pDX12Resource));
    }

    void DX12Graphics::UpdateGraphicsBufferResource(IGraphicsResource* pGraphicsResource, const void* updateSource, GraphicsResourceType graphicsResourceType)
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

            pDX12ConstantBufferView->UpdateResourceBuffer(updateSource);
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

            pDX12VertexBufferView->UpdateResourceBuffer(updateSource);
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

            pDX12IndexBufferView->UpdateResourceBuffer(updateSource);
        }
            break;
        }
    }

    void DX12Graphics::SetConstantBufferResource(UINT index, IGraphicsResource* pGraphicsResource)
    {
        auto pDX12ConstantBufferView = dynamic_cast<DX12ConstantBufferView*>(pGraphicsResource);
        if (pDX12ConstantBufferView == nullptr)
        {
            MessageBoxA(NULL, "DX12ConstantBufferViewのキャストに失敗しました。", "MessageBox", MB_OK);
            return;
        }

        m_pDX12Command->SetGraphicsRootDescriptorTable(index, pDX12ConstantBufferView->GetGPUDescriptorHandle());
    }

    void DX12Graphics::SetShaderResource(UINT index, IGraphicsResource* pGraphicsResource)
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

        m_pDX12Command->DrawInstanced(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, numVerties);
    }

    void DX12Graphics::DrawIndexedIndexBuffer(IGraphicsResource* pVertexResource, IGraphicsResource* pIndexResource, UINT numIndeies)
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

        m_pDX12Command->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, numIndeies);
    }

} // namespace