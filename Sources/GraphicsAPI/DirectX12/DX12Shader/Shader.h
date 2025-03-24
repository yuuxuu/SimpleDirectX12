/**
 * @file Shader.h
 * @brief
 * @author Yu Kimura
 * @date 2020/05/18
 */

#pragma once

#ifndef _SHADER_H_
#define _SHADER_H_

#include "GraphicsAPI/DirectX12/DX12Shader/IShader.h"

using Microsoft::WRL::ComPtr;

namespace Graphics 
{
    class DX12Device;
    class DX12Command;

namespace Shader
{ 
    using shadersMap = std::map<std::string, ComPtr<ID3D10Blob>>;

    class Shader : public IShader
    {
    private:
        ComPtr<ID3D12RootSignature> m_pRootSignature;
        shadersMap                  m_pShadeMap;

        DX12Device*                 pDX12Device;
        DX12Command*                pDX12Commnad;

    private:
        Shader(const Shader&) = delete;
        Shader operator=(const Shader&) = delete;

        void CompileShader(const std::string& shaderFilePath, const std::string& entryPoint, const std::string& shaderModel) override;
    public:
        explicit Shader(DX12Device* pDX12Device, DX12Command* pDX12Commnad);
        ~Shader();

        bool Initialize(const std::string& shaderFilePath) override;

        void SetRootSignature();

        void SetGraphicsPipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc);
        void SetComputePipelineState(D3D12_COMPUTE_PIPELINE_STATE_DESC& computePipelineStateDesc);
    };
} // namespace Shader
} // namespace Graphics

#endif _SHADER_H_
