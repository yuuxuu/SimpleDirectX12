/**
 * @file DX12GraphicsPSO.cpp
 * @brief
 * @author Yu Kimura
 * @date 2020/05/18
 */

#include "GraphicsAPI/DirectX12/DX12Pipline/PSO/DX12GraphicsPSO.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12Command.h"

#include "GraphicsAPI/Shader/Shader.h"

namespace Graphics 
{
    // コンストラクタ
    DX12GraphicsPSO::DX12GraphicsPSO()
    {}

    // デストラクタ
    DX12GraphicsPSO::~DX12GraphicsPSO()
    {}

    bool DX12GraphicsPSO::InitializeRootSignature(DX12Device* pDX12Device, Shader::Shader* pShader, D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc)
    {
        auto m_pShaderMap = pShader->GetShaderMap();
        for (const auto pShader : m_pShaderMap)
        {
            auto shaderByteCode = D3D12_SHADER_BYTECODE
            {
                pShader.second->GetBufferPointer(),
                pShader.second->GetBufferSize()
            };

            if (pShader.first == "VS")
                graphicsPipelineStateDesc.VS = shaderByteCode;
            else if (pShader.first == "PS")
                graphicsPipelineStateDesc.PS = shaderByteCode;
            else if (pShader.first == "HS")
                graphicsPipelineStateDesc.HS = shaderByteCode;
            else if (pShader.first == "DS")
                graphicsPipelineStateDesc.DS = shaderByteCode;
            else if (pShader.first == "GS")
                graphicsPipelineStateDesc.GS = shaderByteCode;
        }

        auto shaderMapItr = m_pShaderMap.find("VS");
        if (shaderMapItr == m_pShaderMap.cend())
            return false;

        auto pShaderBlob = shaderMapItr->second;

        ComPtr<ID3DBlob> pBlob;
        HRESULT hr = D3DGetBlobPart(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &pBlob);
        if (FAILED(hr))
        {
            MessageBoxA(NULL, "RootSignatureの取得に失敗しました。", "MessageBox", MB_OK);
            return false;
        }

        if (!pDX12Device->CreateRootSignature(m_pRootSignature, pBlob.Get()))
        {
            MessageBoxA(NULL, "RootSignatureの作成に失敗しました。", "MessageBox", MB_OK);
            return false;
        }

        graphicsPipelineStateDesc.pRootSignature = m_pRootSignature.Get();

        return true;
    }

    bool DX12GraphicsPSO::InitializePipeline(DX12Device* pDX12Device, D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc)
    {
        if (!pDX12Device->CreateGraphicsPipelineState(m_pPipelineState, graphicsPipelineStateDesc))
        {
            MessageBoxA(NULL, "グラフィックスパイプラインの作成に失敗しました。", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    void DX12GraphicsPSO::SetRootSignature(DX12Command* pDX12Command)
    {
        pDX12Command->SetGraphicsRootSignature(m_pRootSignature.Get());
    }

    void DX12GraphicsPSO::SetPipeline(DX12Command* pDX12Command)
    {
        pDX12Command->SetPipelineState(m_pPipelineState.Get());
    }
} // namespace Graphics