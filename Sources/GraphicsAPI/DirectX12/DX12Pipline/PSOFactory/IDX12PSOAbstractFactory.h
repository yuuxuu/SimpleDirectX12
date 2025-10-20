/**
 * @file IDX12PSOAbstractFactory.h
 * @brief
 * @author Yu Kimura
 * @date 2025/03/31
 */

#pragma once

#include "GraphicsAPI/DirectX12/DX12Pipline/PSO/IDX12PSO.h"

namespace Graphics
{
	namespace Shader
	{
		class Shader;
	}

	class DX12Device;

	class IDX12PSOAbstractFactory
	{
	public:
		IDX12PSOAbstractFactory() {}
		virtual ~IDX12PSOAbstractFactory() {}

		virtual std::unique_ptr<IDX12PSO> CreatePSO(DX12Device* pDX12Device, Shader::Shader* pShader) = 0;
	};
} // namespace Graphics