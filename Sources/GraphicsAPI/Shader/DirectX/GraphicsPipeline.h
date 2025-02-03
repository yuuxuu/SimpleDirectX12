/**
 * @file GraphicsPipeline.h
 * @brief
 * @author Yu Kimura
 * @date 2020/05/18
 */

#pragma once

#ifndef _GRAPHICS_PIPELINE_H_
#define _GRAPHICS_PIPELINE_H_

using Microsoft::WRL::ComPtr;

namespace Graphics 
{
    class DX12Device;
    class DX12Command;

namespace Shader
{ 
    class GraphicsPipeline
    {
    private:
        ComPtr<ID3D12PipelineState> m_pPipelineState;

        DX12Device*                 pDX12Device;
        DX12Command*                pDX12Command;

    private:
        GraphicsPipeline(const GraphicsPipeline&) = delete;
        GraphicsPipeline operator=(const GraphicsPipeline&) = delete;

        void SetDepthStencilState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc);
        void SetRasterizerState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc);
        void SetBlendState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc);
    public:
        explicit GraphicsPipeline(DX12Device* pDX12Device, DX12Command* pDX12Command);
        ~GraphicsPipeline();

        bool InitializePipeline(D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc);

        void SetPipeline();
    };
} // namespace GraphicsPipeline
} // namespace Graphics

#endif // _GRAPHICS_PIPELINE_H_
