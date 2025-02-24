/**
 * @file ModelMesh.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/02/02
 */

#include "ModelMesh.h"

#include "Utility/utility.h"

#include "math/math.h"

#include "Param/BufferParam.h"
#include "Param/ModelDrawInfoParam.h"

#include "Buffer/WorldBuffer.h"
#include "Buffer/WorldBoneMatrixBuffer.h"
#include "Buffer/VertexBuffer.h"

#include "Mesh/Mesh.h"
#include "Mesh/MeshBone.h"
#include "Mesh/MeshAnimation.h"

#include "Material/Material.h"

#include "Texture/Texture.h"

namespace Simple {

    // コンストラクタ
    ModelMesh::ModelMesh() :
        m_ModelDrawInfoParamVec(),
        m_registerMeshVec(),
        m_registerMaterialVec(),
        m_registerTextureMap(),
        pConstantBufferResource(),
        pBoneMatrixConstantBufferResource()
    {}

    // デストラクタ
    ModelMesh::~ModelMesh()
    {}

    void ModelMesh::InitializeGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        BufferParam param;
        param.byteWidth = sizeof(WorldBuffer);
        param.byteWidthStride = sizeof(WorldBuffer);

        pGraphics->InitializeGraphicsBufferResource(pConstantBufferResource, &param, Graphics::GraphicsResourceType::CBV);

        param.byteWidth = sizeof(WorldBoneMatrixBuffer);
        param.byteWidthStride = sizeof(WorldBoneMatrixBuffer);

        pGraphics->InitializeGraphicsBufferResource(pBoneMatrixConstantBufferResource, &param, Graphics::GraphicsResourceType::CBV);

        for (auto itr = m_registerMeshVec.cbegin(); itr != m_registerMeshVec.cend(); itr++)
            itr->get()->InitializeGraphicsResource(pGraphics);

        for (auto itr = m_registerMaterialVec.cbegin(); itr != m_registerMaterialVec.cend(); itr++)
            itr->get()->InitializeGraphicsResource(pGraphics);

        for (auto itr = m_registerTextureMap.cbegin(); itr != m_registerTextureMap.cend(); itr++)
        {
            if (!itr->second->LoadTexture())
                continue;

            itr->second->InitializeGraphicsResource(pGraphics);
        }
    }

    void ModelMesh::UpdateGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        {
            Matrix matScale;
            auto scale = 1.0f;
            matScale.dx_m = DirectX::XMMatrixScaling(scale, scale, scale);

            Matrix matRotate;
            matRotate.dx_m = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);

            Matrix matTrans;
            matTrans.dx_m = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

            Matrix matWorld;
            matWorld.dx_m = matScale.dx_m * matRotate.dx_m * matTrans.dx_m;

            Simple::WorldBuffer worldBuffer;
            worldBuffer.matW.dx_m = DirectX::XMMatrixTranspose(matWorld.dx_m);

            BufferParam param;
            param.byteWidth = sizeof(WorldBuffer);
            param.byteWidthStride = sizeof(WorldBuffer);

            pGraphics->UpdateGraphicsBufferResource(pConstantBufferResource, &worldBuffer, &param, Graphics::GraphicsResourceType::CBV);

            auto meshAnimationItr = m_registerMeshAnimationMap.cbegin();
            if (meshAnimationItr != m_registerMeshAnimationMap.cend())
            {
                meshAnimationItr->second->UpdateFrame();

                Simple::WorldBoneMatrixBuffer worldBoneMatrixBuffer;

                auto boneCount = static_cast<int>(m_registerMeshBoneVec.size());
                for (auto i = 0; i < boneCount; ++i)
                {
                    const auto& boneName = m_registerMeshBoneVec[i]->GetBoneName();

                    const auto& matrix = m_registerMeshBoneVec[i]->GetInitBoneMatrix();
                    //const auto& matrix = meshAnimationItr->second->GetInitMeshAnimationMatrix(boneName);
                    const auto& animationMatrix = meshAnimationItr->second->GetMeshAnimationMatrix(boneName);

                    worldBoneMatrixBuffer.boneMatrixW[i].dx_m = DirectX::XMMatrixTranspose(matrix.dx_m);
                }

                param.byteWidth = sizeof(WorldBoneMatrixBuffer);
                param.byteWidthStride = sizeof(WorldBoneMatrixBuffer);

                pGraphics->UpdateGraphicsBufferResource(pBoneMatrixConstantBufferResource, &worldBoneMatrixBuffer, &param, Graphics::GraphicsResourceType::CBV);
            }
        }
    }

    void ModelMesh::SetGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        pGraphics->SetConstantBufferResource(0, pConstantBufferResource);

        pGraphics->SetConstantBufferResource(6, pBoneMatrixConstantBufferResource);

        for (const auto& meshDrawInfo : m_ModelDrawInfoParamVec)
        {
            if (meshDrawInfo.pMaterial)
            {
                meshDrawInfo.pMaterial->SetGraphicsResource(pGraphics);

                for (auto i = 0; i < meshDrawInfo.vecTexture.size(); ++i)
                    meshDrawInfo.vecTexture[i]->SetGraphicsResource(i, pGraphics);
            }

            meshDrawInfo.pMesh->SetGraphicsResource(pGraphics);
        }
    }

    void ModelMesh::RegisterMesh(std::unique_ptr<Mesh>& pMesh)
    {
        m_registerMeshVec.emplace_back(std::move(pMesh));
    }

    void ModelMesh::RegisterMeshBone(std::unique_ptr<MeshBone>& pMeshBone)
    {
        m_registerMeshBoneVec.emplace_back(std::move(pMeshBone));
    }

    void ModelMesh::RegisterMeshAnimation(const std::string& animationName, std::unique_ptr<MeshAnimation>& pFBXMeshAnimation)
    {
        auto itr = m_registerMeshAnimationMap.find(animationName);
        if (itr != m_registerMeshAnimationMap.cend())
            return;

        m_registerMeshAnimationMap[animationName] = std::move(pFBXMeshAnimation);
    }

    void ModelMesh::RegisterMaterial(std::unique_ptr<Material>& pMaterial)
    {
        m_registerMaterialVec.emplace_back(std::move(pMaterial));
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
        m_ModelDrawInfoParamVec.emplace_back(param);
    }

} // namespace