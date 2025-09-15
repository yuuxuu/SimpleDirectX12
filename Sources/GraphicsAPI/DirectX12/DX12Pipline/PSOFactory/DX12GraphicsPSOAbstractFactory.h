/**
 * @file DX12GraphicsPSOAbstractFactory.h
 * @brief
 * @author Yu Kimura
 * @date 2025/06/19
 */

#pragma once

#include "GraphicsAPI/DirectX12/DX12Pipline/PSOFactory/IDX12PSOAbstractFactory.h"

namespace Graphics
{
	class DX12GraphicsPSOAbstractFactory : IDX12PSOAbstractFactory
	{
	public:
		DX12GraphicsPSOAbstractFactory();
		~DX12GraphicsPSOAbstractFactory();

		std::unique_ptr<IDX12PSO> CreatePSO(DX12Device* pDX12Device, Shader::Shader* pShader) override;
	};
} // namespace Graphics

