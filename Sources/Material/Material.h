/**
 * @file Material.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/05
 */

#pragma once

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "GraphicsAPI/IGraphics.h"
#include "GraphicsAPI/IGraphicsResource.h"

#include "Buffer/MaterialBuffer.h"

namespace Simple 
{
    class Material 
    {
    private:
        MaterialBuffer                  m_MaterialBuffer;

        Graphics::IGraphicsResource*    pGraphicsResource;

    private:
        Material(const Material&) = delete;
        Material operator=(const Material&) = delete;

    public:
        Material();
        ~Material();

        void Initialize(const MaterialBuffer& materialBuffer);

        void InitializeGraphicsResource(Graphics::IGraphics* pGraphics);

        void SetGraphicsResource(Graphics::IGraphics* pGraphics);
    };

} // namespace

#endif // _MATERIAL_H_
