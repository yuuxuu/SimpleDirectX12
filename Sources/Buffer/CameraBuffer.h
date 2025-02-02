/**
 * @file CameraBuffer.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/17
 */

#pragma once

#ifndef _CAMERA_BUFFER_TYPE_H_
#define _CAMERA_BUFFER_TYPE_H_

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

#endif // _CAMERA_BUFFER_TYPE_H_

