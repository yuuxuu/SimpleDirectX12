/**
 * @file WorldBuffer.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/24
 */

#pragma once

#ifndef _WORLD_BONE_MATRIX_BUFFER_TYPE_H_
#define _WORLD_BONE_MATRIX_BUFFER_TYPE_H_

#include "Math/math.h"

namespace Simple
{
    constexpr UINT BONE_MATRIX_MAX = 200;

    struct WorldBoneMatrixBuffer
    {
        std::array<Matrix, BONE_MATRIX_MAX> boneMatrixW;
    };

} // namespace

#endif // _WORLD_BONE_MATRIX_BUFFER_TYPE_H_

