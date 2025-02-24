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
    class MeshBone;
    class Material;
    class Texture;

    struct ModelDrawInfoParam : public IParam
    {
        Mesh*                   pMesh;
        Material*               pMaterial;
        std::vector<Texture*>   vecTexture;

        ModelDrawInfoParam() :
            pMesh(nullptr),
            pMaterial(nullptr)
        {}
    };
} // namespace

#endif // _MODEL_DRAW_INFO_PARAM_H_

