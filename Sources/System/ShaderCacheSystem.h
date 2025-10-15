/**
 * @file ShaderCahceSystem.h
 * @brief
 * @author Yu Kimura
 * @date 2025/07/07
 */

#pragma once

#include "GraphicsAPI/Shader/IShader.h"
#include "GraphicsAPI/DirectX12/DX12Pipline/PSO/IDX12PSO.h"

#include "Param/ShaderParam.h"

namespace Graphics
{
	class DX12Device;
	class DX12Command;
}

namespace Simple
{
namespace System
{
	struct ShaderPSOPair
	{
		std::unique_ptr<Graphics::Shader::IShader>	pShader;
		std::unique_ptr<Graphics::IDX12PSO>			pPSO;

		ShaderPSOPair()
		{ }

		ShaderPSOPair(std::unique_ptr<Graphics::Shader::IShader> pShader, std::unique_ptr<Graphics::IDX12PSO> pPSO):
			pShader(std::move(pShader)),
			pPSO(std::move(pPSO))
		{}
	};

	using ShaderMap = std::map<std::string, ShaderPSOPair>;

	class ShaderCacheSystem
	{
	private:
		ShaderMap m_shaderMap;

		std::pair<std::string, ShaderPSOPair*> m_pCurrentShader;

	private:
		ShaderCacheSystem();
		~ShaderCacheSystem();

	public:
		static ShaderCacheSystem& GetSystem()
		{
			static ShaderCacheSystem system;
			return system;
		}

		void Initialize(Graphics::DX12Device* pDX12Device);

		void SetCurrentShader(const ShaderParam& shaderFileName);

		void SetPipline(Graphics::DX12Command* pDX12Command);
	};
} // namespace System
} // namespace Simple