/**
 * @file TextureDataParam.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#ifndef _TEXTURE_DATA_PARAM_H_
#define _TEXTURE_DATA_PARAM_H_

#include "IParam.h"

namespace Simple
{
    struct TextureDataParam : public IParam
    {
        std::unique_ptr<uint8_t[]>  pData;

        UINT                        PixelsSize;

        UINT                        RowPitch;
        UINT                        SlicePitch;

        UINT                        Width;
        UINT                        Height;

        DXGI_FORMAT                 Format;

        TextureDataParam() :
            pData(nullptr),
            PixelsSize(0),
            RowPitch(0),
            SlicePitch(0),
            Width(0),
            Height(0),
            Format(DXGI_FORMAT_UNKNOWN)
        { }
    };
} // namespace

#endif // _TEXTURE_DATA_PARAM_H_

