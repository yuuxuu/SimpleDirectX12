/**
 * @file Texture.h
 * @brief
 * @author Yu Kimura
 * @date 2021/05/07
 */

#pragma once

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "GraphicsAPI/IGraphics.h"
#include "GraphicsAPI/IGraphicsResource.h"

#include "Param/TextureDataParam.h"
#include "Param/CubeTextureDataParam.h"

namespace Simple 
{
    class Texture 
    {
    private:
        Graphics::IGraphicsResource*            pGraphicsResource;

        std::unique_ptr<TextureDataParam>       m_pTextureParam;

        std::unique_ptr<CubeTextureDataParam>   m_cubeTextureData;

        bool                                    m_isLoaded;

    private:
        Texture(const Texture&) = delete;
        Texture operator=(const Texture&) = delete;

        void TextureConvertToCubeMapTexture();
    public:
        Texture();
        virtual ~Texture();

        bool LoadTexture(const std::string& filePath);

        void InitializeGraphicsResource(Graphics::IGraphics* pGraphics);

        void SetGraphicsResource(Graphics::IGraphics* pGraphics);

        const TextureDataParam* GetParam() const { return m_pTextureParam.get(); }
    };

} // namespace

#endif // _TEXTURE_H_