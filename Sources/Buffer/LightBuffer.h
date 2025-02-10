/**
 * @file LightBuffer.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/28
 */

#pragma once

#ifndef _LIGHT_BUFFER_TYPE_H_
#define _LIGHT_BUFFER_TYPE_H_

#include "Math/math.h"

namespace Simple
{
    struct DirectionalLightBuffer
    {
        VECTOR4 pos;
        VECTOR4 diffuse;
        VECTOR4 specular;
        float intensity;
    };

    struct PointLightBuffer
    {
        VECTOR3 pos;
        float   radius;
        VECTOR4 diffuse;
        VECTOR4 specular;
        VECTOR4 attenuation;
    };

    constexpr UINT LIGHT_MAX = 1000;

    struct PointLightConstantBuffer
    {
        PointLightBuffer    pointLightBuffers[LIGHT_MAX];

        int                 numPointLight;
    };

    struct SpotLightBuffer
    {
        VECTOR3 pos;
        float   angle;
        VECTOR4 diffuse;
        VECTOR4 specular;
        VECTOR4 attenuation;
    };

} // namespace

#endif // _WORLD_BUFFER_TYPE_H_

