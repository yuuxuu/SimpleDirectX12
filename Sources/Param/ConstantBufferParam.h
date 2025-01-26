/**
 * @file ConstantBufferParam.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#ifndef _CONSTANT_BUFFER_PARAM_H_
#define _CONSTANT_BUFFER_PARAM_H_

#include "IParam.h"

namespace Simple
{
    struct ConstantBufferParam : public IParam
    {
        UINT byteWidth;

        ConstantBufferParam() :
            byteWidth()
        { }
    };
} // namespace

#endif // _CONSTANT_BUFFER_PARAM_H_

