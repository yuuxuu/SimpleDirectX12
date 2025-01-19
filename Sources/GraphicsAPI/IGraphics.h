/**
 * @file IGraphics.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/18
 */

#pragma once

#ifndef _IGRAPHICS_H_
#define _IGRAPHICS_H_

namespace Graphics
{
    class IGraphicsResource;

    class IGraphics
    {
    public:
        IGraphics() {}
        virtual ~IGraphics() {}

        virtual bool InitializeGraphics(HWND hwnd, UINT width, UINT height) = 0;

        virtual void FinalizeGraphics() = 0;

        virtual void UpdateGraphics(UINT width, UINT height) = 0;

        virtual void InitializeGraphicsResource(IGraphicsResource*& pGraphicsResource, const UINT byteWidth) = 0;

        virtual void UpdateGraphicsResource(IGraphicsResource* pGraphicsResource, const void* updateSource) = 0;
    };
} // namespace

#endif // _IGRAPHICS_H_

