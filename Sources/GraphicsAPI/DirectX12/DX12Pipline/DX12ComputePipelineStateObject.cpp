/**
 * @file ComputePipeline.cpp
 * @brief
 * @author Yu Kimura
 * @date 2020/05/18
 */

#include "DX12ComputePipelineStateObject.h"

#include "GraphicsAPI/Shader/Shader.h"

namespace Graphics 
{
    // コンストラクタ
    DX12ComputePipelineStateObject::DX12ComputePipelineStateObject()
    {
    }

    // デストラクタ
    DX12ComputePipelineStateObject::~DX12ComputePipelineStateObject()
    {
    }

    void DX12ComputePipelineStateObject::SetComputePipelineState(Shader::Shader* pShader, D3D12_COMPUTE_PIPELINE_STATE_DESC& computePipelineStateDesc)
    {
        auto m_pShaderMap = pShader->GetShaderMap();
        for (const auto pShader : m_pShaderMap)
        {
            auto shaderByteCode = D3D12_SHADER_BYTECODE
            {
                pShader.second->GetBufferPointer(),
                pShader.second->GetBufferSize()
            };

            if (pShader.first == "CS")
                computePipelineStateDesc.CS = shaderByteCode;
        }
    }
} // namespace Graphics