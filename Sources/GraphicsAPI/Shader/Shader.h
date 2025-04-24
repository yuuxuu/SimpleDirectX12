/**
 * @file Shader.h
 * @brief
 * @author Yu Kimura
 * @date 2020/05/18
 */

#pragma once

#ifndef _SHADER_H_
#define _SHADER_H_

#include "GraphicsAPI/Shader/IShader.h"

using Microsoft::WRL::ComPtr;

namespace Graphics 
{
namespace Shader
{ 
    using shadersMap = std::map<std::string, ComPtr<ID3D10Blob>>;

    class Shader : public IShader
    {
    private:
        shadersMap m_pShadeMap;

    private:
        Shader(const Shader&) = delete;
        Shader operator=(const Shader&) = delete;

        void CompileShader(const std::string& shaderFilePath, const std::string& entryPoint, const std::string& shaderModel) override;
    public:
        explicit Shader();
        ~Shader();

        bool Initialize(const std::string& shaderFilePath) override;

        const shadersMap& GetShaderMap() const;
    };
} // namespace Shader
} // namespace Graphics

#endif _SHADER_H_
