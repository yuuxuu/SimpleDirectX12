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
#include "GraphicsAPI/DirectX12/DX12Resource/CBV/DX12ConstantBurfferView.h"

namespace Graphics
{
    DX12Graphics::DX12Graphics()
    {}

    DX12Graphics::~DX12Graphics()
    {}

    bool DX12Graphics::InitializeGraphics(HWND hwnd, UINT windowWidth, UINT windowHeight)
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

        m_pDX12GraphicsResourceBuilder = std::make_unique<DX12GraphicsResourceBuilder>();

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

                    m_pDX12GraphicsResourceBuilder->CreateRenderTargetView(
                        m_pDX12Device.get(),
                        heapAllocator.get(),
                        windowWidth,
                        windowHeight,
                        renderTargetView);

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
                m_pDX12GraphicsResourceBuilder->CreateDepthStencilView(
                    m_pDX12Device.get(),
                    heapAllocator.get(),
                    windowWidth,
                    windowHeight,
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
                D3D12_DESCRIPTOR_HEAP_FLAG_NONE
            };

            auto heapAllocator = std::make_unique<DX12HeapAllocator>(m_pDX12Device.get(), descriptorHeapDesc);
            if (heapAllocator->CreateDescriptorHeap())
                m_pDX12HeapAllocatorMap[descriptorHeapDesc.Type] = std::move(heapAllocator);
        }

        return true;
    }

    void DX12Graphics::FinalizeGraphics()
    {

    }

    void DX12Graphics::UpdateGraphics(const UINT windowWidth, const UINT windowHeight)
    {
        auto index = m_pDX12Device->GetCurrentBackBufferIndex();

        auto renderTargetView = static_cast<DX12RenderTargetView*>(m_pRenderTargetViews.at(index).get());
        auto renderTargetViewHandle = renderTargetView->GetDescriptorHandle();
        auto depthStencilView = static_cast<DX12DepthStencilView*>(m_pDepthStencilView.get());
        auto depthStencilViewHandle = depthStencilView->GetDescriptorHandle();

        m_pDX12Command->ResetCommandList();

        m_pDX12Command->SetRect(windowWidth, windowHeight);
        m_pDX12Command->SetViewPort(0, 0, windowWidth, windowHeight);

        m_pDX12Command->SetResourceBarrier(renderTargetView->GetResouce(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        m_pDX12Command->SetResourceBarrier(depthStencilView->GetResouce(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

        m_pDX12Command->ClearRenderTargetView(renderTargetViewHandle);
        m_pDX12Command->ClearDepthStencilView(depthStencilViewHandle, D3D12_CLEAR_FLAG_DEPTH);
        m_pDX12Command->SetRenderTargetView(&renderTargetViewHandle, &depthStencilViewHandle);

        m_pDX12Command->SetResourceBarrier(renderTargetView->GetResouce(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        m_pDX12Command->SetResourceBarrier(depthStencilView->GetResouce(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);

        m_pDX12Command->ExecuteComandLists();

        m_pDX12Device->Present();
    }

    void DX12Graphics::InitializeGraphicsResource(IGraphicsResource*& pGraphicsResource, const UINT byteWidth)
    {
        auto pHeapAllocatorItr = m_pDX12HeapAllocatorMap.find(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        if (pHeapAllocatorItr == m_pDX12HeapAllocatorMap.end())
        {
            return;
        }

        std::unique_ptr<IDX12Resouce> pDX12Resource;
        m_pDX12GraphicsResourceBuilder->CreateConstantBufferView(m_pDX12Device.get(), pHeapAllocatorItr->second.get(), byteWidth, pDX12Resource);

        pGraphicsResource = pDX12Resource.get();

        m_pConstantBufferViews.push_back(std::move(pDX12Resource));
    }

    void DX12Graphics::UpdateGraphicsResource(IGraphicsResource* pGraphicsResource, const void* updateSource)
    {
        auto pDX12ConstantBufferView = static_cast<DX12ConstantBufferView*>(pGraphicsResource);
        if (pDX12ConstantBufferView == nullptr)
            return;

        pDX12ConstantBufferView->UpdateResourceBuffer(updateSource);
    }

} // namespace