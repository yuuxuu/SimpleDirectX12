/**
 * @file DX12Graphics.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#ifndef _DX12_GRAPHICS_H_
#define _DX12_GRAPHICS_H_

#include "GraphicsAPI/IGraphics.h"

namespace Graphics
{
    class DX12Device;
    class DX12Command;
    class DX12HeapAllocator;
    class DX12RenderTargetView;
    class DX12DepthStencilView;

    class DX12Graphics : public IGraphics
    {
    private:
        std::unique_ptr<DX12Device>                         m_pDX12Device;
        std::unique_ptr<DX12Command>                        m_pDX12Command;

        std::vector<std::unique_ptr<DX12HeapAllocator>>     m_pHeapAllocators;

        std::vector<std::unique_ptr<DX12RenderTargetView>>  m_pRenderTargetViews;
        std::unique_ptr<DX12DepthStencilView>               m_pDepthStencilView;
    private:
        DX12Graphics(const DX12Graphics&) = delete;
        DX12Graphics& operator=(const DX12Graphics) = delete;

    public:
        DX12Graphics();
        ~DX12Graphics();

        bool InitializeGraphics(HWND hwnd, UINT width, UINT height) override;

        void FinalizeGraphics() override;

        void UpdateGraphics(UINT width, UINT height) override;
    };
} // namespace

#endif // _DX12_GRAPHICS_H_
