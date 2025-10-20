/**
 * @file ShaderCahceSystem.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/07/07
 */

#include "ShaderCacheSystem.h"

#include "GraphicsAPI/Shader/Shader.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12HeapAllocator.h"

#include "GraphicsAPI/DirectX12/DX12Pipline/PSOFactory/DX12GraphicsPSOAbstractFactory.h"

namespace Simple
{
namespace System
{
	ShaderCacheSystem::ShaderCacheSystem()
	{ }

	ShaderCacheSystem::~ShaderCacheSystem()
	{ }

	void ShaderCacheSystem::Initialize(Graphics::DX12Device* pDX12Device)
	{
        std::map<std::string, std::filesystem::path> filePathMap;

        auto itr = std::filesystem::recursive_directory_iterator("Resources/Shader/HLSL");
        for (const auto& file : itr)
        {
            auto extension = file.path().extension();
            if (extension != ".hlsl")
                continue;

            filePathMap[file.path().filename().string()] = file.path();
        }

        Graphics::DX12GraphicsPSOAbstractFactory factory;
        for (const auto& fileName : Simple::ShaderFileNameList)
        {
            auto itr = filePathMap.find(fileName);
            if (itr == filePathMap.cend())
            {
                continue;
            }

            auto pShader = std::make_unique<Graphics::Shader::Shader>();
            if (!pShader->Initialize(itr->second.string()))
                return;

            if (auto pPSO = factory.CreatePSO(pDX12Device, pShader.get()))
            {
                m_shaderMap[fileName] = ShaderPSOPair(std::move(pShader), std::move(pPSO));
            }
        }
	}

    void ShaderCacheSystem::SetCurrentShader(const ShaderParam& shaderParam)
    {
        if (m_pCurrentShader.first.targetShaderFile != shaderParam.targetShaderFile)
        {
            auto itr = m_shaderMap.find(shaderParam.targetShaderFile);
            if (itr == m_shaderMap.cend())
            {
                m_pCurrentShader = {};

                MessageBoxA(NULL, (shaderParam.targetShaderFile + " が読み込まれていません。").c_str(), "MessageBox", MB_OK);
                return;
            }

            m_pCurrentShader = { shaderParam, &itr->second };
        }
        else if(m_pCurrentShader.first.SetupResources != shaderParam.SetupResources)
        {
            m_pCurrentShader.first = shaderParam;
        }
    }

    void ShaderCacheSystem::SetPipline(Graphics::IGraphics* pGraphics, Graphics::DX12Command* pDX12Command)
    {
        if (!m_pCurrentShader.second)
        {
            return;
        }

        m_pCurrentShader.second->pPSO->SetRootSignature(pDX12Command);
        m_pCurrentShader.second->pPSO->SetPipeline(pDX12Command);

        for (const auto& resourceMap : m_pCurrentShader.first.SetupResources)
        {
            pGraphics->SetConstantBufferResource(resourceMap.first, resourceMap.second);
        }
    }

} // namespace System
} // namespace Simple