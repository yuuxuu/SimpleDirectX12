/**
 * @file DX12GraphicsPSO.h
 * @brief
 * @author Yu Kimura
 * @date 2020/05/18
 */

#pragma once

#include "GraphicsAPI/DirectX12/DX12Pipline/PSO/IDX12PSO.h"

using Microsoft::WRL::ComPtr;

namespace Graphics 
{
    namespace Shader
    {
        class Shader;
    }

    class DX12Device;

    class DX12GraphicsPSO : public IDX12PSO
    {
    private:
        ComPtr<ID3D12PipelineState> m_pPipelineState;
        ComPtr<ID3D12RootSignature> m_pRootSignature;

    private:
        DX12GraphicsPSO(const DX12GraphicsPSO&) = delete;
        DX12GraphicsPSO operator=(const DX12GraphicsPSO&) = delete;

    public:
        DX12GraphicsPSO();
        ~DX12GraphicsPSO();

        bool InitializeRootSignature(DX12Device* pDX12Device, Shader::Shader* pShader, D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc);

        bool InitializePipeline(DX12Device* pDX12Device, D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc);

        void SetRootSignature(DX12Command* pDX12Command) override;

        void SetPipeline(DX12Command* pDX12Commnad) override;
    };
} // namespace Graphics
