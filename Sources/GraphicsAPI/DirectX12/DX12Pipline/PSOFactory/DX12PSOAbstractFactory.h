/**
 * @file DX12PSOAbstractFactory.h
 * @brief
 * @author Yu Kimura
 * @date 2025/06/19
 */

#pragma once

#include "GraphicsAPI/DirectX12/DX12Pipline/PSOFactory/IDX12PSOAbstractFactory.h"

namespace Graphics
{
	class DX12PSOAbstractFactory : IDX12PSOAbstractFactory
	{
	public:
		DX12PSOAbstractFactory();
		~DX12PSOAbstractFactory();

		std::unique_ptr<IDX12PSO> CreatePSO(DX12Device* pDX12Device, Shader::Shader* pShader) override;
	};
} // namespace Graphics

