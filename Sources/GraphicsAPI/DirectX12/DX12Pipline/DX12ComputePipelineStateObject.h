/**
 * @file DX12ComputePipelineStateObject.h
 * @brief
 * @author Yu Kimura
 * @date 2020/05/18
 */

#pragma once

#ifndef _DX12_COMPUTE_PIPELINE_STATE_OBJECT_H_
#define _DX12_COMPUTE_PIPELINE_STATE_OBJECT_H_

namespace Graphics
{
    namespace Shader
    {
        class Shader;
    }

    class DX12ComputePipelineStateObject
    {
    private:

    private:
        DX12ComputePipelineStateObject(const DX12ComputePipelineStateObject&) = delete;
        DX12ComputePipelineStateObject operator=(const DX12ComputePipelineStateObject&) = delete;

        void SetComputePipelineState(Shader::Shader* pShader, D3D12_COMPUTE_PIPELINE_STATE_DESC& computePipelineStateDesc);

    public:
        DX12ComputePipelineStateObject();
        ~DX12ComputePipelineStateObject();
    };
} // namespace Graphics

#endif // _DX12_COMPUTE_PIPELINE_STATE_OBJECT_H_
