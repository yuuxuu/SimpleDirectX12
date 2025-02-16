/**
 * @file FBXLoader.cpp
 * @brief
 * @author 木村優
 * @date 2021/11/07
 */

#include "FBXLoader.h"

#include "math/math.h"

#include "Mesh/Mesh.h"
#include "Mesh/ModelMesh.h"

#include "Material/Material.h"

#include "Texture/Texture.h"

#include "Buffer/VertexBuffer.h"

#include "Param/ModelDrawInfoParam.h"

namespace Simple 
{
namespace ModelLoader
{
    // コンストラクタ
    FBXLoader::FBXLoader()
    {}

    // デストラクタ
    FBXLoader::~FBXLoader()
    {}

    // FbxMeshをコンバート
    void FBXLoader::ConvertMesh(FbxMesh* pFbxMesh, Simple::ModelMesh* pModelMesh, ModelDrawInfoParam* pParam)
    {
        auto pMesh = std::make_unique<Simple::Mesh>();
        pParam->pMesh = pMesh.get();

        std::cout << "メッシュ名 = " << pFbxMesh->GetName() << std::endl;

        std::vector<Simple::VertexBuffer> vertices;
        std::vector<DWORD> indices;
        std::vector<VECTOR2> vecUV;
        std::vector<VECTOR3> vecNormal;

        ConvertVertex(pFbxMesh, vertices, indices);
        ConvertNormal(pFbxMesh, vertices, indices, vecNormal);
        ConvertColor(pFbxMesh, vertices, indices);
        ConvertUV(pFbxMesh, vertices, indices, vecUV);

        if (vecUV.empty() && vecNormal.empty())
        {
            pMesh->Initialize(vertices, indices);
            pModelMesh->RegisterMesh(pMesh);
            return;
        }

        // 頂点インデックス数と各要素のインデックス数が違う場合は頂点の再構築

        std::vector<Simple::VertexBuffer> vecVertex;
        auto numIndex = indices.size();
        vecVertex.reserve(numIndex);
        for (auto i = 0; i < numIndex; ++i)
        {
            auto vertex = vertices[indices[i]];

            if(!vecUV.empty())
                vertex.uv = vecUV[i];

            if (!vecNormal.empty())
                vertex.normal = vecNormal[indices[i]];

            vecVertex.push_back(vertex);
        }
        
        pMesh->Initialize(vecVertex, std::vector<DWORD>());
        pModelMesh->RegisterMesh(pMesh);
    }

    void FBXLoader::ConvertVertex(FbxMesh* pFbxMesh, std::vector<Simple::VertexBuffer>& vertices, std::vector<DWORD>& indices)
    {
        auto numVertex = pFbxMesh->GetControlPointsCount();
        std::cout << "総頂点数 = " << numVertex << std::endl;

        auto numPolygon = pFbxMesh->GetPolygonCount();
        std::cout << "総ポリゴン数 = " << numPolygon << std::endl;

        auto numIndex = pFbxMesh->GetPolygonVertexCount();
        std::cout << "総インデックス数 = " << numIndex << std::endl;

        auto pVertices = pFbxMesh->GetPolygonVertices();
        indices.reserve(numIndex);
        for (auto i = 0; i < numIndex; ++i)
            indices.push_back(static_cast<int>(pVertices[i]));

        auto points = pFbxMesh->GetControlPoints();
        vertices.reserve(numVertex);
        for (auto i = 0; i < numVertex; ++i)
        {
            VertexBuffer vertex;
            vertex.vertex.x = static_cast<float>(points[i][0]);
            vertex.vertex.y = static_cast<float>(points[i][1]);
            vertex.vertex.z = static_cast<float>(points[i][2]);

            vertices.push_back(vertex);
        }
    }

    void FBXLoader::ConvertNormal(FbxMesh* pFbxMesh, std::vector<Simple::VertexBuffer>& vertices, const std::vector<DWORD>& indices, std::vector<VECTOR3> vecNormal)
    {
        auto numNormalLayer = pFbxMesh->GetElementNormalCount();
        for (auto i = 0; i < numNormalLayer; ++i)
        {
            auto pFbxNormal = pFbxMesh->GetElementNormal(i);
            auto pFbxTangent = pFbxMesh->GetElementTangent(i);
            auto pFbxBinormal = pFbxMesh->GetElementBinormal(i);

            auto numNormal = pFbxNormal->GetDirectArray().GetCount();
            std::cout << "法線数 = " << numNormal << std::endl;

            auto numNormalIndex = pFbxNormal->GetIndexArray().GetCount();
            std::cout << "法線インデックス数 = " << numNormalIndex << std::endl;

            numNormalIndex = numNormalIndex < 1 ? (int)indices.size() : numNormalIndex;

            vecNormal.reserve(numNormalIndex);

            auto mappingMode = pFbxNormal->GetMappingMode();
            auto referenceMode = pFbxNormal->GetReferenceMode();

            if (mappingMode == FbxGeometryElement::eByControlPoint)
            {
                if (referenceMode == FbxGeometryElement::eDirect)
                {
                    for (auto j = 0; j < numNormal; ++j)
                    {
                        if (pFbxNormal)
                        {
                            vertices[j].normal.x = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(j)[0]);
                            vertices[j].normal.y = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(j)[1]);
                            vertices[j].normal.z = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(j)[2]);
                        }
                        if (pFbxTangent)
                        {
                            vertices[j].tangent.x = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(j)[0]);
                            vertices[j].tangent.y = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(j)[1]);
                            vertices[j].tangent.z = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(j)[2]);
                        }
                        if (pFbxBinormal)
                        {
                            vertices[j].binormal.x = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(j)[0]);
                            vertices[j].binormal.y = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(j)[1]);
                            vertices[j].binormal.z = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(j)[2]);
                        }
                    }
                }
            }
            else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
            {
                if (referenceMode == FbxGeometryElement::eDirect)
                {
                    for (auto j = 0; j < numNormalIndex; ++j)
                    {
                        auto index = indices[j];

                        if (pFbxNormal)
                        {
                            vertices[index].normal.x = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(j)[0]);
                            vertices[index].normal.y = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(j)[1]);
                            vertices[index].normal.z = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(j)[2]);

                            vecNormal.push_back(vertices[index].normal);
                        }
                        if (pFbxTangent)
                        {
                            vertices[index].tangent.x = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(j)[0]);
                            vertices[index].tangent.y = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(j)[1]);
                            vertices[index].tangent.z = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(j)[2]);
                        }
                        if (pFbxBinormal)
                        {
                            vertices[index].binormal.x = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(j)[0]);
                            vertices[index].binormal.y = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(j)[1]);
                            vertices[index].binormal.z = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(j)[2]);
                        }
                    }
                }
                else if (referenceMode == FbxGeometryElement::eIndexToDirect)
                {
                    for (auto j = 0; j < numNormalIndex; ++j)
                    {
                        auto index = pFbxNormal->GetIndexArray().GetAt(j);

                        if (pFbxNormal)
                        {
                            vertices[indices[j]].normal.x = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(index)[0]);
                            vertices[indices[j]].normal.y = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(index)[1]);
                            vertices[indices[j]].normal.z = static_cast<float>(pFbxNormal->GetDirectArray().GetAt(index)[2]);
                        }
                        if (pFbxTangent)
                        {
                            vertices[indices[j]].tangent.x = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(index)[0]);
                            vertices[indices[j]].tangent.y = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(index)[1]);
                            vertices[indices[j]].tangent.z = static_cast<float>(pFbxTangent->GetDirectArray().GetAt(index)[2]);
                        }
                        if (pFbxBinormal)
                        {
                            vertices[indices[j]].binormal.x = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(index)[0]);
                            vertices[indices[j]].binormal.y = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(index)[1]);
                            vertices[indices[j]].binormal.z = static_cast<float>(pFbxBinormal->GetDirectArray().GetAt(index)[2]);
                        }
                    }
                }
            }
        }
    }

    void FBXLoader::ConvertColor(FbxMesh* pFbxMesh, std::vector<Simple::VertexBuffer>& vertices, const std::vector<DWORD>& indices)
    {
        auto numColorLayer = pFbxMesh->GetElementVertexColorCount();
        for (auto i = 0; i < numColorLayer; ++i)
        {
            auto pFbxColor = pFbxMesh->GetElementVertexColor(i);

            auto numColor = pFbxColor->GetDirectArray().GetCount();
            std::cout << "頂点カラー数 = " << numColor << std::endl;

            auto numColorIndex = pFbxColor->GetIndexArray().GetCount();
            std::cout << "頂点カラーインデクッス数 = " << numColor << std::endl;

            auto mappingMode = pFbxColor->GetMappingMode();
            auto referenceMode = pFbxColor->GetReferenceMode();

            if (mappingMode == FbxGeometryElement::eByControlPoint)
            {
                if (referenceMode == FbxGeometryElement::eDirect)
                {
                    for (auto j = 0; j < numColor; ++j)
                    {
                        vertices[j].color.x = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[0]);
                        vertices[j].color.y = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[1]);
                        vertices[j].color.z = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[2]);
                        vertices[j].color.w = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[3]);
                    }
                }
            }
            else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
            {
                if (referenceMode == FbxGeometryElement::eDirect)
                {
                    for (auto j = 0; j < numColorIndex; ++j)
                    {
                        auto index = indices[j];

                        vertices[index].color.x = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[0]);
                        vertices[index].color.y = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[1]);
                        vertices[index].color.z = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[2]);
                        vertices[index].color.w = static_cast<float>(pFbxColor->GetDirectArray().GetAt(j)[3]);
                    }
                }
                else if (referenceMode == FbxGeometryElement::eIndexToDirect)
                {
                    for (auto j = 0; j < numColorIndex; ++j)
                    {
                        auto index = pFbxColor->GetIndexArray().GetAt(j);

                        vertices[indices[j]].color.x = static_cast<float>(pFbxColor->GetDirectArray().GetAt(index)[0]);
                        vertices[indices[j]].color.y = static_cast<float>(pFbxColor->GetDirectArray().GetAt(index)[1]);
                        vertices[indices[j]].color.z = static_cast<float>(pFbxColor->GetDirectArray().GetAt(index)[2]);
                        vertices[indices[j]].color.w = static_cast<float>(pFbxColor->GetDirectArray().GetAt(index)[3]);
                    }
                }
            }
        }
    }

    void FBXLoader::ConvertUV(FbxMesh* pFbxMesh, std::vector<Simple::VertexBuffer>& vertices, const std::vector<DWORD>& indices, std::vector<VECTOR2>& vecUV)
    {
        auto numUVLayer = pFbxMesh->GetElementUVCount();
        numUVLayer = numUVLayer > 1 ? 1 : numUVLayer;

        for (auto i = 0; i < numUVLayer; ++i)
        {
            auto pFbxUV = pFbxMesh->GetElementUV(i);

            auto numUV = pFbxUV->GetDirectArray().GetCount();
            std::cout << "頂点UV数 = " << numUV << std::endl;

            auto numUVIndex = pFbxUV->GetIndexArray().GetCount();
            std::cout << "頂点UVインデクッス数 = " << numUVIndex << std::endl;

            vecUV.reserve(numUVIndex);

            std::cout << "UVセット名 = " << pFbxUV->GetName() << std::endl;

            auto mappingMode = pFbxUV->GetMappingMode();
            auto referenceMode = pFbxUV->GetReferenceMode();

            if (mappingMode == FbxGeometryElement::eByControlPoint)
            {
                if (referenceMode == FbxGeometryElement::eDirect)
                {
                    for (auto j = 0; j < numUV; ++j) 
                    {
                        vertices[j].uv.x = static_cast<float>(pFbxUV->GetDirectArray().GetAt(j)[0]);
                        vertices[j].uv.y = 1.0f - static_cast<float>(pFbxUV->GetDirectArray().GetAt(j)[1]);
                    }
                }
                else if (referenceMode == FbxGeometryElement::eIndexToDirect)
                {
                    for (auto j = 0; j < numUV; ++j)
                    {
                        auto index = static_cast<int>(indices[j]);

                        vertices[j].uv.x = static_cast<float>(pFbxUV->GetDirectArray().GetAt(index)[0]);
                        vertices[j].uv.y = 1.0f - static_cast<float>(pFbxUV->GetDirectArray().GetAt(index)[1]);
                    }
                }
            }
            else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
            {
                if (referenceMode == FbxGeometryElement::eDirect)
                {
                    for (auto j = 0; j < numUVIndex; ++j) 
                    {
                        vertices[indices[j]].uv.x = static_cast<float>(pFbxUV->GetDirectArray().GetAt(j)[0]);
                        vertices[indices[j]].uv.y = 1.0f - static_cast<float>(pFbxUV->GetDirectArray().GetAt(j)[1]);
                    }
                }
                else if (referenceMode == FbxGeometryElement::eIndexToDirect)
                {
                    for (auto j = 0; j < numUVIndex; ++j) 
                    {
                        auto index = pFbxUV->GetIndexArray().GetAt(j);

                        vertices[indices[j]].uv.x = static_cast<float>(pFbxUV->GetDirectArray().GetAt(index)[0]);
                        vertices[indices[j]].uv.y = 1.0f - static_cast<float>(pFbxUV->GetDirectArray().GetAt(index)[1]);

                        vecUV.push_back(vertices[indices[j]].uv);
                    }
                }
            }
        }
    }

    void FBXLoader::ConvertMaterial(FbxSurfaceMaterial* pFbxMaterial, Simple::ModelMesh* pModelMesh, ModelDrawInfoParam* pParam)
    {
        std::cout << "マテリアル名 = " << pFbxMaterial->GetName() << std::endl;

        if (std::string("PictureBorder") == pFbxMaterial->GetName())
            std::cout << "マテリアル名 = " << pFbxMaterial->GetName() << std::endl;

        MaterialBuffer materialBuffer;

        if (pFbxMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId)) 
        {
            auto lambert = static_cast<FbxSurfaceLambert*>(pFbxMaterial);

            materialBuffer.diffuse.x = static_cast<float>(lambert->Diffuse.Get()[0]);
            materialBuffer.diffuse.y = static_cast<float>(lambert->Diffuse.Get()[1]);
            materialBuffer.diffuse.z = static_cast<float>(lambert->Diffuse.Get()[2]);
            materialBuffer.diffuse.w = 1.0f;

            materialBuffer.ambient.x = static_cast<float>(lambert->Ambient.Get()[0]);
            materialBuffer.ambient.y = static_cast<float>(lambert->Ambient.Get()[1]);
            materialBuffer.ambient.z = static_cast<float>(lambert->Ambient.Get()[2]);
            materialBuffer.ambient.w = 1.0f;

            materialBuffer.emissive.x = static_cast<float>(lambert->Emissive.Get()[0]);
            materialBuffer.emissive.y = static_cast<float>(lambert->Emissive.Get()[1]);
            materialBuffer.emissive.z = static_cast<float>(lambert->Emissive.Get()[2]);
            materialBuffer.emissive.w = 1.0f;
        }
        else if (pFbxMaterial->GetClassId().Is(FbxSurfacePhong::ClassId)) 
        {
            auto phong = static_cast<FbxSurfacePhong*>(pFbxMaterial);

            materialBuffer.diffuse.x = static_cast<float>(phong->Diffuse.Get()[0]);
            materialBuffer.diffuse.y = static_cast<float>(phong->Diffuse.Get()[1]);
            materialBuffer.diffuse.z = static_cast<float>(phong->Diffuse.Get()[2]);
            materialBuffer.diffuse.w = 1.0f;

            materialBuffer.specular.x = static_cast<float>(phong->Specular.Get()[0]);
            materialBuffer.specular.y = static_cast<float>(phong->Specular.Get()[1]);
            materialBuffer.specular.z = static_cast<float>(phong->Specular.Get()[2]);
            materialBuffer.specular.w = 1.0f;

            materialBuffer.ambient.x = static_cast<float>(phong->Ambient.Get()[0]);
            materialBuffer.ambient.y = static_cast<float>(phong->Ambient.Get()[1]);
            materialBuffer.ambient.z = static_cast<float>(phong->Ambient.Get()[2]);
            materialBuffer.ambient.w = 1.0f;

            materialBuffer.emissive.x = static_cast<float>(phong->Emissive.Get()[0]);
            materialBuffer.emissive.y = static_cast<float>(phong->Emissive.Get()[1]);
            materialBuffer.emissive.z = static_cast<float>(phong->Emissive.Get()[2]);
            materialBuffer.emissive.w = 1.0f;
        }

        auto pMaterial = std::make_unique<Simple::Material>(materialBuffer);
        pParam->pMaterial = pMaterial.get();

        pModelMesh->RegisterMaterial(pMaterial);

        std::vector<std::string> properties =
        {
            std::string(FbxSurfaceMaterial::sDiffuse),
            std::string(FbxSurfaceMaterial::sNormalMap),
        };

        std::vector<Texture*> textureVec;
        for (auto prop : properties)
        {
            auto pFbxProp = pFbxMaterial->FindProperty(prop.c_str());
            auto numTexture = pFbxProp.GetSrcObjectCount<FbxFileTexture>();
            for (auto i = 0; i < numTexture; ++i)
            {
                auto pFbxTexture = pFbxProp.GetSrcObject<FbxFileTexture>(i);
                if (!pFbxTexture) continue;

                std::cout << "テクスチャ名 = " << pFbxTexture->GetRelativeFileName() << std::endl;

                auto parentPath = std::filesystem::path(pFbxTexture->GetRelativeFileName()).parent_path().string();
                auto path = parentPath + "/textures/" + std::string(pFbxTexture->GetFileName());

                auto pTexture = std::make_unique<Simple::Texture>(path);
                textureVec.push_back(pTexture.get());

                pModelMesh->RegisterTexture(pFbxTexture->GetFileName(), pTexture);
            }
        }

        pParam->vecTexture.swap(textureVec);
    }

    // FbxNodeを再帰的に検索
    void FBXLoader::RecursiveFbxNode(FbxNode* pRootNode, std::vector<FbxNode*>& outFbxNodes)
    {
        auto numChild = pRootNode->GetChildCount();
        for (auto i = 0; i < numChild; ++i)
        {
            auto pChildNode = pRootNode->GetChild(i);
            if (!pChildNode)
                continue;

            auto pFbxNodeAttribute = pChildNode->GetNodeAttribute();
            if (!pFbxNodeAttribute)
            {
                RecursiveFbxNode(pChildNode, outFbxNodes);
                continue;
            }

            if (pFbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
            {
                auto pFbxMesh = pChildNode->GetMesh();
                if (!pFbxMesh)
                    continue;

                outFbxNodes.emplace_back(pChildNode);
            }

            RecursiveFbxNode(pChildNode, outFbxNodes);
        }
    }

    // FBXモデルの読み込み
    bool FBXLoader::LoadModel(const std::string& filePath, Simple::ModelMesh* pModelMesh)
    {
        auto fbxManager = FbxManager::Create();

        auto fbxIOSetting = FbxIOSettings::Create(fbxManager, IOSROOT);

        auto fbxInporter = FbxImporter::Create(fbxManager, "");
        if (!fbxInporter->Initialize(filePath.c_str(), -1, fbxManager->GetIOSettings())) {
            std::cout << filePath << "= 読み込み：失敗" << std::endl;
            return false;
        }

        auto fbxScene = FbxScene::Create(fbxManager, "");
        fbxInporter->Import(fbxScene);
        fbxInporter->Destroy();

        // 軸の設定
        auto ourAxisSystem = FbxAxisSystem::DirectX;

        // DirectX系に変換
        auto sceneAxisSystem = fbxScene->GetGlobalSettings().GetAxisSystem();
        if (ourAxisSystem != sceneAxisSystem) 
        {
            FbxAxisSystem::DirectX.ConvertScene(fbxScene);
        }

        FbxGeometryConverter converter(fbxManager);
        if (!converter.Triangulate(fbxScene, true))
        {
            std::cout << "三角ポリゴンへのコンバートに失敗しました。" << std::endl;
        }

        // 単位系の統一
        auto SceneSystemUnit = fbxScene->GetGlobalSettings().GetSystemUnit();
        if (SceneSystemUnit.GetScaleFactor() != 1.0f)
        {
            // センチメーター単位にコンバートする。
            FbxSystemUnit::cm.ConvertScene(fbxScene);
        }

        auto pRootNode = fbxScene->GetRootNode();
        if (!pRootNode) return false;

        std::vector<FbxNode*> fbxMeshNodes;

        RecursiveFbxNode(pRootNode, fbxMeshNodes);

        for (const auto& pNode : fbxMeshNodes)
        {
            auto pFbxMesh = pNode->GetMesh();
            if (!pFbxMesh) continue;

            ModelDrawInfoParam param;

            ConvertMesh(pFbxMesh, pModelMesh, &param);

            auto numMaterial = pNode->GetMaterialCount();
            for (auto i = 0; i < numMaterial; ++i)
            {
                auto pFbxMaterial = pNode->GetMaterial(i);
                if (!pFbxMaterial) continue;

                ConvertMaterial(pFbxMaterial, pModelMesh, &param);
            }

            pModelMesh->AddModelDrawInfoParam(param);
        }

        return true;
    }
} // namespace ModelLoader
} // namespace Simple
