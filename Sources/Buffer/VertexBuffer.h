/**
 * @file WorldBuffer.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#pragma once

#ifndef _VERTEX_BUFFER_TYPE_H_
#define _VERTEX_BUFFER_TYPE_H_

#include "Math/math.h"

namespace Simple
{
    constexpr UINT BONE_INDEX_MAX = 4;

    struct VertexBuffer
    {
        VECTOR3 vertex;
        VECTOR4 color;
        VECTOR2 uv;
        VECTOR3 normal;
        VECTOR3 tangent;
        VECTOR3 binormal;
        std::array<int, BONE_INDEX_MAX>     boneIndices{ -1, -1, -1, -1 };
        std::array<float, BONE_INDEX_MAX>   boneWeights{ .0f, .0f, .0f, .0f};
    };

    struct MeshInfo
    {
        VECTOR3 vertexE;
        VECTOR3 vertexCenterPos;
        VECTOR3 maxVertexPos;
        VECTOR3 minVertexPos;

        MeshInfo() :
            vertexE(0.0f, 0.0f, 0.0f),
            vertexCenterPos(0.0f, 0.0f, 0.0f),
            maxVertexPos(0.0f, 0.0f, 0.0f),
            minVertexPos(10000.0f, 10000.0f, 10000.0f)
        {
        }
    };

} // namespace

#endif // _WORLD_BUFFER_TYPE_H_

