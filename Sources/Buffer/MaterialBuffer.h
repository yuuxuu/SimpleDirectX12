/**
 * @file WorldBuffer.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#pragma once

#ifndef _MATERIAL_BUFFER_TYPE_H_
#define _MATERIAL_BUFFER_TYPE_H_

#include "Math/math.h"

namespace Simple
{
    struct MaterialBuffer
    {
        VECTOR4	diffuse;
        VECTOR4	specular;
        VECTOR4	ambient;
        VECTOR4	emissive;

        MaterialBuffer() :
            diffuse(1.0f, 1.0f, 1.0f, 1.0f),
            specular(),
            ambient(),
            emissive()
        {}
    };

} // namespace

#endif // _MATERIAL_BUFFER_TYPE_H_

