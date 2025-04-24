/**
 * @file DX12GraphicsPipelineStateObject.h
 * @brief
 * @author Yu Kimura
 * @date 2020/05/18
 */

#pragma once

#ifndef _DX12_GRAPHICS_PIPELINE_STATE_OBJECT__H_
#define _DX12_GRAPHICS_PIPELINE_STATE_OBJECT__H_

using Microsoft::WRL::ComPtr;

namespace Graphics 
{
    namespace Shader
    {
        class Shader;
    }

    class DX12Device;
    class DX12Command;

    class DX12GraphicsPipelineStateObject
    {
    private:
        ComPtr<ID3D12PipelineState> m_pPipelineState;
        ComPtr<ID3D12RootSignature> m_pRootSignature;

        DX12Device* pDX12Device;
        DX12Command* pDX12Command;

    private:
        DX12GraphicsPipelineStateObject(const DX12GraphicsPipelineStateObject&) = delete;
        DX12GraphicsPipelineStateObject operator=(const DX12GraphicsPipelineStateObject&) = delete;

        void SetGraphicsPipelineState(Shader::Shader* pShader, D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc);

        void SetDepthStencilState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc);
        void SetRasterizerState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc);
        void SetBlendState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc);
    public:
        explicit DX12GraphicsPipelineStateObject(DX12Device* pDX12Device, DX12Command* pDX12Command);
        ~DX12GraphicsPipelineStateObject();

        bool InitializePipeline(Shader::Shader* pShader, D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc);

        void SetRootSignature();

        void SetPipeline();
    };
} // namespace Graphics

#endif // _DX12_GRAPHICS_PIPELINE_STATE_OBJECT__H_
