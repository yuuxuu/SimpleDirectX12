/**
 * @file IDX12PSO.h
 * @brief
 * @author Yu Kimura
 * @date 2025/03/31
 */

#pragma once

namespace Graphics
{
	class DX12Command;

	class IDX12PSO
	{
	public:
		IDX12PSO() {}
		virtual ~IDX12PSO() {}

		virtual void SetRootSignature(DX12Command* pDX12Command) = 0;

		virtual void SetPipeline(DX12Command* pDX12Command) = 0;
	};
} // namespace Graphics