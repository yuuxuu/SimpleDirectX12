/**
 * @file ShaderParam.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#include "IParam.h"

namespace Simple
{
    static const std::string& ShaderFileName_Model = "Model.hlsl";
    static const std::string& ShaderFileName_ModelLine = "ModelLine.hlsl";
    static const std::string& ShaderFileName_Sprite = "Sprite.hlsl";

    static std::vector<std::string> ShaderFileNameList
    {
        ShaderFileName_Model,
        ShaderFileName_ModelLine,
        ShaderFileName_Sprite,
    };

    struct ShaderParam : public IParam
    {
        std::string targetShaderFile;
    };
} // namespace

