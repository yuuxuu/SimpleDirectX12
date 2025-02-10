/**
 * @file ModelDrawInfoParam.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#ifndef _MODEL_DRAW_INFO_PARAM_H_
#define _MODEL_DRAW_INFO_PARAM_H_

#include "IParam.h"

namespace Simple
{
    class Mesh;
    class Material;
    class Texture;

    using MaterialTexturesMap = std::map<Material*, std::vector<Texture*>>;

    struct ModelDrawInfoParam : public IParam
    {
        Mesh*                 pMesh;
        MaterialTexturesMap   pMaterialTexturesMap;
    };
} // namespace

#endif // _MODEL_DRAW_INFO_PARAM_H_

