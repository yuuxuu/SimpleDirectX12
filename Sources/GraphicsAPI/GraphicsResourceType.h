/**
 * @file GraphicsResourceType.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#ifndef _GRAPHICS_RESOURCE_TYPE_H_
#define _GRAPHICS_RESOURCE_TYPE_H_

namespace Graphics
{
    enum class GraphicsResourceType
    {
        CBV,
        SRV,
        RTV,
        DSV,
        VBV,
        IBV,
    };
} // namespace

#endif // _GRAPHICS_RESOURCE_TYPE_H_

