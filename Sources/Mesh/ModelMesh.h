/**
 * @file ModelMesh.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/02
 */

#pragma once

#ifndef _MODEL_MESH_H_
#define _MODEL_MESH_H_

#include "ModelLoader/IModelLoader.h"

#include "GraphicsAPI/IGraphics.h"
#include "GraphicsAPI/IGraphicsResource.h"

#include "ModelLoader/IModelLoader.h"

namespace Simple 
{
    class Mesh;
    class Material;
    class Texture;

    struct ModelDrawInfoParam;

    using RegisterMeshVec = std::vector<std::unique_ptr<Simple::Mesh>>;
    using RegisterMaterialVec = std::vector<std::unique_ptr<Simple::Material>>;
    using RegisterTextureMap = std::map<std::string, std::unique_ptr<Simple::Texture>>;

    class ModelMesh 
    {
    private:
        std::vector<ModelDrawInfoParam> m_ModelDrawInfoParamVec;

        RegisterMeshVec                 m_registerMeshVec;
        RegisterMaterialVec             m_registerMaterialVec;
        RegisterTextureMap              m_registerTextureMap;

        Graphics::IGraphicsResource*    pConstantBufferResource;

    private:
        ModelMesh(const ModelMesh&) = delete;
        ModelMesh operator=(const ModelMesh&) = delete;

    public:
        explicit ModelMesh();
        ~ModelMesh();

        void InitializeGraphicsResource(Graphics::IGraphics* pGraphics);

        void SetGraphicsResource(Graphics::IGraphics* pGraphics);

        void RegisterMesh(std::unique_ptr<Mesh>& pMesh);
        void RegisterMaterial(std::unique_ptr<Material>& pMaterial);
        void RegisterTexture(const std::string& texturePath, std::unique_ptr<Texture>& pTexture);

        void AddModelDrawInfoParam(const ModelDrawInfoParam& param);
    };

} // namespace

#endif // _MODEL_MESH_H_
