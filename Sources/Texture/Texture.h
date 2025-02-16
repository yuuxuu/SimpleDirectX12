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
        std::unique_ptr<TextureDataParam>       m_pTextureParam;

        std::unique_ptr<CubeTextureDataParam>   m_cubeTextureData;

        std::string                             m_filePath;

        bool                                    m_isLoaded;

        Graphics::IGraphicsResource*            pGraphicsResource;

    private:
        Texture(const Texture&) = delete;
        Texture operator=(const Texture&) = delete;

        bool LoadTexture(const std::string& filePath);

        void TextureConvertToCubeMapTexture();
    public:
        explicit Texture();
        explicit Texture(const std::string& filePath);
        virtual ~Texture();

        bool LoadTexture();

        void InitializeGraphicsResource(Graphics::IGraphics* pGraphics);

        void SetGraphicsResource(const UINT index, Graphics::IGraphics* pGraphics);

        const TextureDataParam* GetParam() const { return m_pTextureParam.get(); }
    };

} // namespace

#endif // _TEXTURE_H_