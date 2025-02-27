/**
 * @file ShaderFactory.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/25
 */

#pragma once

#ifndef _SHADER_FACTORY_H_
#define _SHADER_FACTORY_H_

#include "Param/IParam.h"

namespace Graphics
{
    class DX12Device;
    class DX12Command;

namespace Shader
{
    class Shader;

    class IShaderFactory
    {
    public:
        IShaderFactory() {};
        virtual ~IShaderFactory() {};

        virtual std::unique_ptr<Shader> CreateShader(DX12Device* pDX12Device, DX12Command* pDX12Command) = 0;
    };

    class ModelShaderFactory : public IShaderFactory
    {
    public:
        ModelShaderFactory() {}
        ~ModelShaderFactory() {}

        std::unique_ptr<Shader> CreateShader(DX12Device* pDX12Device, DX12Command* pDX12Command) override;
    };

    class ModelSkinAnimationShaderFactory : public IShaderFactory
    {
    public:
        ModelSkinAnimationShaderFactory() {}
        ~ModelSkinAnimationShaderFactory() {}

        std::unique_ptr<Shader> CreateShader(DX12Device* pDX12Device, DX12Command* pDX12Command) override;
    };

} // namespace Shader
} // namespace Graphics

#endif // _SHADER_FACTORY_H_
