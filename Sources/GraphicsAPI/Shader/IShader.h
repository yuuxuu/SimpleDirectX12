/**
 * @file IShader.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/29
 */

#pragma once

#ifndef _ISHADER_H_
#define _ISHADER_H_

namespace Graphics 
{
namespace Shader
{
    class IShader 
    {
    protected:
        IShader() {};
        virtual ~IShader() {};

        virtual bool Initialize(const std::string& shaderFilePath) = 0;

        virtual void CompileShader(const std::string& shaderFilePath, const std::string& entryPoint, const std::string& shaderModel) = 0;
    };
} // namespace Shader
} // namespace Graphics

#endif // _ISHADER_H_
