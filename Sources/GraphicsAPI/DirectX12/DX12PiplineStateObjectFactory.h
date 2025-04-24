/**
 * @file DX12PiplineStateObjectFactory.h
 * @brief
 * @author Yu Kimura
 * @date 2025/03/31
 */

#pragma once

#ifndef _DX12_PIPLINE_STATE_OBJECT_BUILDER_
#define _DX12_PIPLINE_STATE_OBJECT_BUILDER_

namespace Graphics
{
	// 抽象ファクトリーになるので各クラスの具象クラスを追加する
	// パイプラインステートオブジェクト毎に各ファクトリーで使用パターンを生成する

	class IRootSignatureFactory
	{
	public:
		IRootSignatureFactory() {}
		virtual ~IRootSignatureFactory() {}
	};

	class IDepthStencilStateObjectFactory
	{
	public:
		IDepthStencilStateObjectFactory(){}
		virtual ~IDepthStencilStateObjectFactory(){}
	};

	class IRasterizerStateObjectFactory
	{
	public:
		IRasterizerStateObjectFactory() {}
		virtual ~IRasterizerStateObjectFactory() {}
	};

	class IBlendStateObjectFactory
	{
	public:
		IBlendStateObjectFactory() {}
		virtual ~IBlendStateObjectFactory() {}
	};

	class IInputElementFactory
	{
	public:
		IInputElementFactory() {}
		virtual ~IInputElementFactory() {}
	};


	class DX12PiplineStateObjectFactory
	{
	public:


	private:
		DX12PiplineStateObjectFactory(const DX12PiplineStateObjectFactory&) = delete;
		DX12PiplineStateObjectFactory operator=(const DX12PiplineStateObjectFactory&) = delete;

	public:
		DX12PiplineStateObjectFactory();
		~DX12PiplineStateObjectFactory();

	};
} // namespace Graphics

#endif // _DX12_PIPLINE_STATE_OBJECT_BUILDER_