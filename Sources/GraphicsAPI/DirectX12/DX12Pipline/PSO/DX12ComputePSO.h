/**
 * @file DX12ComputePSO.h
 * @brief
 * @author Yu Kimura
 * @date 2020/05/18
 */

#pragma once

#include "GraphicsAPI/DirectX12/DX12Pipline/PSO/IDX12PSO.h"

namespace Graphics
{
    namespace Shader
    {
        class Shader;
    }

    class DX12ComputePSO : public IDX12PSO
    {
    private:

    private:
        DX12ComputePSO(const DX12ComputePSO&) = delete;
        DX12ComputePSO operator=(const DX12ComputePSO&) = delete;

        void SetComputePipelineState(Shader::Shader* pShader, D3D12_COMPUTE_PIPELINE_STATE_DESC& computePipelineStateDesc);

    public:
        DX12ComputePSO();
        ~DX12ComputePSO();

        void SetRootSignature(DX12Command* pDX12Command) override { throw std::runtime_error("実装がない関数です。"); };

        void SetPipeline(DX12Command* pDX12Commnad) override { throw std::runtime_error("実装がない関数です。"); };
    };
} // namespace Graphics
