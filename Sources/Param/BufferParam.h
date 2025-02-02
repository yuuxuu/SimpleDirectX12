/**
 * @file BufferParam.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#ifndef _BUFFER_PARAM_H_
#define _BUFFER_PARAM_H_

#include "IParam.h"

namespace Simple
{
    struct BufferParam : public IParam
    {
        UINT byteWidth;
        UINT byteWidthStride;

        BufferParam() :
            byteWidth(),
            byteWidthStride()
        { }
    };
} // namespace

#endif // _CONSTANT_BUFFER_PARAM_H_

