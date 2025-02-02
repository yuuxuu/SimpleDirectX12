/**
 * @file Shader.cpp
 * @brief
 * @author Yu Kimura
 * @date 2020/05/18
 */

#include "Shader.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12Command.h"

#include "Utility/utility.h"

namespace Graphics 
{
namespace Shader
{
    // コンストラクタ
    Shader::Shader(DX12Device* pDX12Device) :
        pDX12Device(pDX12Device),
        m_pShadeMap(),
        m_pRootSignature()
    {}

    // デストラクタ
    Shader::~Shader()
    {}

    // シェーダーコンパイル
    void Shader::CompileShader(const std::string& shaderFilePath, const std::string& entryPoint, const std::string& shaderModel)
    {
        DWORD shaderFlags = 0;
#ifdef _DEBUG
        shaderFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        ComPtr<ID3D10Blob> pBlob;
        ComPtr<ID3DBlob> pErrBlob;

        std::wstring filePath;
        Simple::StringConvertToWchar(shaderFilePath, filePath);

        HRESULT hr = D3DCompileFromFile(
            filePath.c_str(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            (entryPoint + "_main").c_str(),
            shaderModel.c_str(),
            shaderFlags,
            0,
            &pBlob,
            &pErrBlob);

        if (FAILED(hr)) {
            std::cout << filePath.c_str() << " " << (char*)pErrBlob->GetBufferPointer() << "コンパイル:失敗" << std::endl;
            return;
        }

        m_pShadeMap[entryPoint] = std::move(pBlob);
    }

    // 初期化
    bool Shader::Initialize(const std::string& shaderFilePath)
    {
        static const std::vector<std::pair<std::string, std::string>> SHADER_ENTRY =
        {
            { "VS", "vs_5_0" },
            { "PS", "ps_5_0" },
            { "HS", "hs_5_0" },
            { "DS", "ds_5_0" },
            { "GS", "gs_5_0" },
            { "CS", "cs_5_0" },
        };

        for (const auto entry : SHADER_ENTRY)
            CompileShader(shaderFilePath, entry.first, entry.second);

        if (m_pShadeMap.empty())
        {
            MessageBoxA(NULL, "Shaderの初期化に失敗しました。", "MessageBox", MB_OK);
            return false;
        }

        auto shaderMapItr = m_pShadeMap.find("VS");
        if (shaderMapItr == m_pShadeMap.cend())
            return false;

        auto pShader = shaderMapItr->second;

        ComPtr<ID3DBlob> pBlob;
        HRESULT hr = D3DGetBlobPart(pShader->GetBufferPointer(), pShader->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &pBlob);
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

        return true;
    }

    void Shader::SetRootSignature(DX12Command* pDX12Commnad)
    {
        pDX12Commnad->SetGraphicsRootSignature(m_pRootSignature.Get());
    }

    void Shader::SetGraphicsPipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc)
    {
        for (const auto pShader : m_pShadeMap)
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

        graphicsPipelineStateDesc.pRootSignature = m_pRootSignature.Get();
        graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    }

    void Shader::SetComputePipelineState(D3D12_COMPUTE_PIPELINE_STATE_DESC& computePipelineStateDesc)
    {
        for (const auto pShader : m_pShadeMap)
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
} // namespace Shader
} // namespace Graphics