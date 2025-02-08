/**
 * @file ModelMesh.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/02
 */

#pragma once

#ifndef _MODEL_MESH_H_
#define _MODEL_MESH_H_

#include "Mesh/Mesh.h"
#include "Material/Material.h"
#include "Texture/Texture.h"

#include "Param/ModelDrawInfoParam.h"

namespace Simple 
{
    using RegisterMeshVec = std::vector<std::unique_ptr<Mesh>>;
    using RegisterMaterialVec = std::vector<std::unique_ptr<Material>>;
    using RegisterTextureMap = std::map<std::string, std::unique_ptr<Texture>>;

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

        void LoadModel(const std::string& modelFilePath);

        void InitializeGraphicsResource(Graphics::IGraphics* pGraphics);

        void SetGraphicsResource(Graphics::IGraphics* pGraphics);

        void RegisterMesh(std::unique_ptr<Mesh>& pMesh);
        void RegisterMaterial(std::unique_ptr<Material>& pMaterial);
        void RegisterTexture(const std::string& texturePath, std::unique_ptr<Texture>& pTexture);

        void AddModelDrawInfoParam(const ModelDrawInfoParam& param);
    };

} // namespace

#endif // _MODEL_MESH_H_
