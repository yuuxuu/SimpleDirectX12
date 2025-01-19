/**
 * @file DX12ConstantBufferView.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#pragma once

#ifndef _CONSTANT_BUFFER_TYPE_H_
#define _CONSTANT_BUFFER_TYPE_H_

#include "Math/math.h"

namespace Simple
{
    struct CameraBuffer
    {
        Simple::Matrix  matVP;
        Simple::VECTOR3 cameraPos;
        float           dummy;
        Simple::Matrix  matView;
        Simple::Matrix  matProj;
        Simple::Matrix  invMatProj;
    };

} // namespace

#endif // _CONSTANT_BUFFER_TYPE_H_

