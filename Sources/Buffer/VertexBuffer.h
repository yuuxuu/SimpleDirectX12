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
    struct VertexBuffer
    {
        VECTOR3	vertex;
        VECTOR4	color;
        VECTOR2	uv;
        VECTOR3	normal;
        VECTOR3	tangent;
        VECTOR3	binormal;
    };

} // namespace

#endif // _WORLD_BUFFER_TYPE_H_

