/**
 * @file ShaderFactory.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/02/25
 */

#include "ShaderFactory.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12Command.h"

#include "GraphicsAPI/Shader/DirectX/Shader.h"

namespace Graphics
{
namespace Shader
{
	std::unique_ptr<Shader> ModelShaderFactory::CreateShader(DX12Device* pDX12Device, DX12Command* pDX12Command)
	{
		auto filePath = "Resources/Shader/HLSL/model.hlsl";
		if (!std::filesystem::exists(filePath))
			return nullptr;

		auto pShader = std::make_unique<Shader>(pDX12Device, pDX12Command);
		if (!pShader->Initialize(filePath))
			return nullptr;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc = {};

		pShader->InitializeGraphicsPipelineState(graphicsPipelineStateDesc);

		return std::move(pShader);
	}

	std::unique_ptr<Shader> ModelSkinAnimationShaderFactory::CreateShader(DX12Device* pDX12Device, DX12Command* pDX12Command)
	{
		auto filePath = "Resources/Shader/HLSL/ModelSkinAnimation.hlsl";
		if (!std::filesystem::exists(filePath))
			return nullptr;

		auto pShader = std::make_unique<Shader>(pDX12Device, pDX12Command);
		if (!pShader->Initialize(filePath))
			return nullptr;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc = {};

		pShader->InitializeGraphicsPipelineState(graphicsPipelineStateDesc);

		return std::move(pShader);
	}

} // namespace Shader
} // namespace Graphics