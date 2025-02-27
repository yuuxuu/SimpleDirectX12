/**
 * @file Shader.h
 * @brief
 * @author Yu Kimura
 * @date 2020/05/18
 */

#pragma once

#ifndef _SHADER_H_
#define _SHADER_H_

#include "GraphicsAPI/Shader/IShader.h"

using Microsoft::WRL::ComPtr;

namespace Graphics 
{
    class DX12Device;
    class DX12Command;

namespace Shader
{ 
    class GraphicsPipeline;

    using ShadersMap = std::map<std::string, ComPtr<ID3D10Blob>>;
    using GraphicsPiplineMap = std::map<UINT, std::unique_ptr<GraphicsPipeline>>;

    class Shader : public IShader
    {
    private:
        ComPtr<ID3D12RootSignature> m_pRootSignature;
        ShadersMap                  m_pShadeMap;

        GraphicsPiplineMap          m_pGraphicsPiplineMap;

        DX12Device*                 pDX12Device;
        DX12Command*                pDX12Command;

    private:
        Shader(const Shader&) = delete;
        Shader operator=(const Shader&) = delete;

        void CompileShader(const std::string& shaderFilePath, const std::string& entryPoint, const std::string& shaderModel) override;
    public:
        explicit Shader(DX12Device* pDX12Device, DX12Command* pDX12Commnad);
        ~Shader();

        bool Initialize(const std::string& shaderFilePath) override;

        void SetGraphicsRootSignature();
        void SetComputeRootSignature();

        void SetPiplineState(const UINT primitiveTopologyType);

        void InitializeGraphicsPipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc);
        void InitializeComputePipelineState(D3D12_COMPUTE_PIPELINE_STATE_DESC& computePipelineStateDesc);
    };
} // namespace Shader
} // namespace Graphics

#endif _SHADER_H_
