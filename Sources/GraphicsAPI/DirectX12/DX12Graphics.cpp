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

#include "GraphicsAPI/DirectX12/DX12Resource/RTV/DX12RenderTargetView.h"
#include "GraphicsAPI/DirectX12/DX12Resource/DSV/DX12DepthStencilView.h"

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
                    auto renderTargetView = std::make_unique<DX12RenderTargetView>(m_pDX12Device.get(), heapAllocator.get(), nullptr);
                    renderTargetView->Initialize(nullptr, nullptr, i);

                    m_pRenderTargetViews.push_back(std::move(renderTargetView));
                }

                m_pHeapAllocators.push_back(std::move(heapAllocator));
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
                D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilviewDesc = {};

                depthStencilviewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
                depthStencilviewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
                depthStencilviewDesc.Flags = D3D12_DSV_FLAG_NONE;
                depthStencilviewDesc.Texture2D.MipSlice = 0;

                D3D12_RESOURCE_DESC resourceDesc = {};

                resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                resourceDesc.Width = windowWidth;
                resourceDesc.Height = windowHeight;
                resourceDesc.DepthOrArraySize = 1;
                resourceDesc.MipLevels = 1;
                resourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
                resourceDesc.SampleDesc = { 1 , 0 };
                resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
                resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

                CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

                m_pDepthStencilView = std::make_unique<DX12DepthStencilView>(m_pDX12Device.get(), heapAllocator.get(), &depthStencilviewDesc);
                m_pDepthStencilView->Initialize(&prop, &resourceDesc);

                m_pHeapAllocators.push_back(std::move(heapAllocator));
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
        }

        return true;
    }

    void DX12Graphics::FinalizeGraphics()
    {

    }

    void DX12Graphics::UpdateGraphics(UINT windowWidth, UINT windowHeight)
    {
        m_pDX12Command->ResetCommandList();

        m_pDX12Command->SetRect(windowWidth, windowHeight);
        m_pDX12Command->SetViewPort(0, 0, windowWidth, windowHeight);

        auto index = m_pDX12Device->GetCurrentBackBufferIndex();
        auto renderTargetView = m_pRenderTargetViews.at(index).get();

        m_pDX12Command->SetResourceBarrier(renderTargetView->GetResouce(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        m_pDX12Command->SetResourceBarrier(m_pDepthStencilView->GetResouce(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);

        auto renderTargetViewHandle = renderTargetView->GetDescriptorHandle();
        auto depthStencilViewHandle = m_pDepthStencilView->GetDescriptorHandle();

        m_pDX12Command->ClearRenderTargetView(renderTargetViewHandle);
        m_pDX12Command->ClearDepthStencilView(depthStencilViewHandle, D3D12_CLEAR_FLAG_DEPTH);
        m_pDX12Command->SetRenderTargetView(&renderTargetViewHandle, &depthStencilViewHandle);

        m_pDX12Command->SetResourceBarrier(renderTargetView->GetResouce(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        m_pDX12Command->SetResourceBarrier(m_pDepthStencilView->GetResouce(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);

        m_pDX12Command->ExecuteComandLists();

        m_pDX12Device->Present();
    }

} // namespace