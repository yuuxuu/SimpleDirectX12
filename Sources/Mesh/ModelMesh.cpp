/**
 * @file ModelMesh.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/02/02
 */

#include "ModelMesh.h"

#include "Utility/utility.h"

#include "ModelLoader/FBX/FBXLoader.h"

#include "Param/BufferParam.h"

#include "Buffer/WorldBuffer.h"

namespace Simple {

    // コンストラクタ
    ModelMesh::ModelMesh() :
        m_ModelDrawInfoParamVec(),
        m_registerMeshVec(),
        m_registerMaterialVec(),
        m_registerTextureMap(),
        pConstantBufferResource()
    {}

    // デストラクタ
    ModelMesh::~ModelMesh()
    {}

    void ModelMesh::LoadModel(const std::string& modelFilePath)
    {
        Simple::ModelLoader::FBXLoader fbxLoader;
        fbxLoader.LoadModel(modelFilePath, this);
    }

    void ModelMesh::InitializeGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        BufferParam param;
        param.byteWidth = sizeof(WorldBuffer);
        param.byteWidthStride = sizeof(WorldBuffer);

        pGraphics->InitializeGraphicsBufferResource(pConstantBufferResource, &param, Graphics::GraphicsResourceType::CBV);

        Matrix matScale;
        auto scale = 1.0f;
        matScale.dx_m = DirectX::XMMatrixScaling(scale, scale, scale);

        Matrix matRotate;
        matRotate.dx_m = DirectX::XMMatrixRotationRollPitchYaw(30.0f, 0.0f, 0.0f);

        Matrix matTrans;
        matTrans.dx_m = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

        Matrix matWorld;
        matWorld.dx_m = matScale.dx_m * matRotate.dx_m * matTrans.dx_m;

        Simple::WorldBuffer worldBuffer;
        worldBuffer.matW.dx_m = DirectX::XMMatrixTranspose(matWorld.dx_m);

        pGraphics->UpdateGraphicsBufferResource(pConstantBufferResource, &worldBuffer, &param, Graphics::GraphicsResourceType::CBV);

        for (auto itr = m_registerMeshVec.cbegin(); itr != m_registerMeshVec.cend(); itr++)
            itr->get()->InitializeGraphicsResource(pGraphics);

        for (auto itr = m_registerMaterialVec.cbegin(); itr != m_registerMaterialVec.cend(); itr++)
            itr->get()->InitializeGraphicsResource(pGraphics);

        for (auto itr = m_registerTextureMap.cbegin(); itr != m_registerTextureMap.cend(); itr++)
        {
            if (!itr->second->LoadTexture(itr->first))
                continue;

            itr->second->InitializeGraphicsResource(pGraphics);
        }
    }

    void ModelMesh::SetGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        pGraphics->SetConstantBufferResource(0, pConstantBufferResource);

        for (auto meshDrawInfo : m_ModelDrawInfoParamVec)
        {
            for (auto material : meshDrawInfo.pMaterialVec)
            {
                material.first->SetGraphicsResource(pGraphics);

                for (auto i = 0; i < material.second.size(); ++i)
                    material.second[i]->SetGraphicsResource(i, pGraphics);
            }

            meshDrawInfo.pMesh->SetGraphicsResource(pGraphics);
        }
    }

    void ModelMesh::RegisterMesh(std::unique_ptr<Mesh>& pMesh)
    {
        m_registerMeshVec.push_back(std::move(pMesh));
    }

    void ModelMesh::RegisterMaterial(std::unique_ptr<Material>& pMaterial)
    {
        m_registerMaterialVec.push_back(std::move(pMaterial));
    }

    void ModelMesh::RegisterTexture(const std::string& texturePath, std::unique_ptr<Texture>& pTexture)
    {
        auto itr = m_registerTextureMap.find(texturePath);
        if (itr != m_registerTextureMap.cend())
            return;

        m_registerTextureMap[texturePath] = std::move(pTexture);
    }

    void ModelMesh::AddModelDrawInfoParam(const ModelDrawInfoParam& param)
    {
        m_ModelDrawInfoParamVec.push_back(param);
    }

} // namespace