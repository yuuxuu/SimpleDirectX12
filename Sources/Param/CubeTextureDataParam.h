/**
 * @file CubeTextureDataParam.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#ifndef _CUBE_TEXTURE_DATA_PARAM_H_
#define _CUBE_TEXTURE_DATA_PARAM_H_

#include "IParam.h"

namespace Simple
{
    constexpr UINT CUBE_FACE_MAX = 6;

    using Datas = std::array<std::unique_ptr<uint8_t[]>, CUBE_FACE_MAX>;

    struct CubeTextureDataParam : public IParam
    {
        Datas       pDatas;

        UINT        PixelsSize;

        UINT        RowPitch;
        UINT        SlicePitch;

        UINT        Width;
        UINT        Height;

        DXGI_FORMAT Format;
    };
} // namespace

#endif // _CUBE_TEXTURE_DATA_PARAM_H_

