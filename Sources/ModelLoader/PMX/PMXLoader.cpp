/**
 * @file PMXLoader.cpp
 * @brief
 * @author 木村優
 * @date 2025/02/14
 */

#include "PMXLoader.h"

#include "math/math.h"

#include "Utility/utility.h"

#include "PMXModelInfo.h"

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
    PMXLoader::PMXLoader()
    {}

    // デストラクタ
    PMXLoader::~PMXLoader()
    {}

    // モデルの読み込み
    bool PMXLoader::LoadModel(const std::string& filePath, Simple::ModelMesh* pModelMesh)
    {
        std::ifstream ifs;
        ifs.open(filePath, std::ios_base::in | std::ios_base::binary);
        if (!ifs)
            return false;

        PmxHeader header;
        {
            ifs.read(reinterpret_cast<char*>(&header.magicNo), sizeof(PmxHeader::magicNo));
            ifs.read(reinterpret_cast<char*>(&header.version), sizeof(PmxHeader::version));
            ifs.read(reinterpret_cast<char*>(&header.dateByteSize), sizeof(PmxHeader::dateByteSize));
            ifs.read(reinterpret_cast<char*>(&header.encood), sizeof(PmxHeader::encood));
            ifs.read(reinterpret_cast<char*>(&header.adduvSize), sizeof(PmxHeader::adduvSize));
            ifs.read(reinterpret_cast<char*>(&header.vertexIndexSize), sizeof(PmxHeader::vertexIndexSize));
            ifs.read(reinterpret_cast<char*>(&header.textureIndexSize), sizeof(PmxHeader::textureIndexSize));
            ifs.read(reinterpret_cast<char*>(&header.materialIndexSize), sizeof(PmxHeader::materialIndexSize));
            ifs.read(reinterpret_cast<char*>(&header.boneIndexSize), sizeof(PmxHeader::boneIndexSize));
            ifs.read(reinterpret_cast<char*>(&header.moorfIndexSize), sizeof(PmxHeader::moorfIndexSize));
            ifs.read(reinterpret_cast<char*>(&header.rigidBodyIndexSize), sizeof(PmxHeader::rigidBodyIndexSize));
        }

        auto getTextBuffer = [&](PmxTextBuffer& textBuffer)
            {
                ifs.read(reinterpret_cast<char*>(&textBuffer.pmxInfoLength), sizeof(PmxTextBuffer::pmxInfoLength));

                if (header.encood == ENCOORD_TYPE::UTF16)
                {
                    std::wstring wstr(textBuffer.pmxInfoLength, '\0');
                    ifs.read(reinterpret_cast<char*>(&wstr[0]), sizeof(BYTE) * textBuffer.pmxInfoLength);

                    UTF16ConvertToUTF8(wstr, textBuffer.pmxInfoStr);
                }
                else
                {
                    ifs.read(reinterpret_cast<char*>(&textBuffer.pmxInfoStr), sizeof(BYTE) * textBuffer.pmxInfoLength);
                }
            };


        PmxModelInfo modelInfo;
        {
            getTextBuffer(modelInfo.pmxModelNameJa);
            getTextBuffer(modelInfo.pmxModelNameEn);
            getTextBuffer(modelInfo.commentJa);
            getTextBuffer(modelInfo.commentEn);
        }

        PmxVertexInfo vertexInfo;
        {
            ifs.read(reinterpret_cast<char*>(&vertexInfo.pmxVertexCount), sizeof(PmxVertexInfo::pmxVertexCount));

            auto LoadVertices = [&]()
                {
					int vertexCount = vertexInfo.pmxVertexCount;
					for (int i = 0; i < vertexCount; ++i) 
					{
                        PmxVertex pmxVertex;

						ifs.read(reinterpret_cast<char*>(&pmxVertex.vertex), sizeof(PmxVertex::vertex));
						ifs.read(reinterpret_cast<char*>(&pmxVertex.normal), sizeof(PmxVertex::normal));
						ifs.read(reinterpret_cast<char*>(&pmxVertex.uv), sizeof(PmxVertex::uv));

						for (int i = 0; i < header.adduvSize; i++) 
							ifs.read(reinterpret_cast<char*>(&pmxVertex.adduv), sizeof(PmxVertex::adduv));

						ifs.read(reinterpret_cast<char*>(&pmxVertex.weightType), sizeof(PmxVertex::weightType));
						
						auto readSize = (header.boneIndexSize == 1) ? sizeof(BYTE) : sizeof(USHORT);

						switch (pmxVertex.weightType) 
						{
						case BONE_TYPE::BDEF1:
							ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneIndex0), readSize);

							pmxVertex.weight.pmxBoneWeight0 = 1.0f;
							break;
						case BONE_TYPE::BDEF2:
                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneIndex0), readSize);
                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneIndex1), readSize);

							ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneWeight0), sizeof(float));
							pmxVertex.weight.pmxBoneWeight1 = 1.0f - pmxVertex.weight.pmxBoneWeight0;
							break;
						case BONE_TYPE::BDEF4:
                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneIndex0), readSize);
                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneIndex1), readSize);
                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneIndex2), readSize);
                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneIndex3), readSize);

                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneWeight0), sizeof(float));
                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneWeight1), sizeof(float));
                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneWeight2), sizeof(float));
                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneWeight3), sizeof(float));
							break;
						case BONE_TYPE::SDEF:
                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneIndex0), readSize);
                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneIndex1), readSize);

                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.pmxBoneWeight0), sizeof(float));
							pmxVertex.weight.pmxBoneWeight1 = 1.0f - pmxVertex.weight.pmxBoneWeight0;
							
                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.SDEF_C), sizeof(PmxBoneWeight::SDEF_C));
                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.SDEF_R0), sizeof(PmxBoneWeight::SDEF_R0));
                            ifs.read(reinterpret_cast<char*>(&pmxVertex.weight.SDEF_R1), sizeof(PmxBoneWeight::SDEF_R1));
							break;
						}

						ifs.read(reinterpret_cast<char*>(&pmxVertex.edge), sizeof(float));

						vertexInfo.vecPmxVertex.push_back(pmxVertex);
					}
                };

            LoadVertices();
        }

        PmxVertexIndexInfo indexInfo;
        {
            ifs.read(reinterpret_cast<char*>(&indexInfo.pmxPolygonCount), sizeof(PmxVertexIndexInfo::pmxPolygonCount));
            indexInfo.pmxPolygonCount /= 3;

            auto LoadIndices = [&]()
                {
                    int polygonCount = indexInfo.pmxPolygonCount;
                    for (int i = 0; i < polygonCount; ++i)
                    {
                        PmxVertexIndex_us16 pmxVertexIndex_us16;
                        PmxVertexIndex_ui32 pmxVertexIndex_ui32;

                        if (header.vertexIndexSize == 1) 
                        {
                            ifs.read(reinterpret_cast<char*>(&pmxVertexIndex_us16.indices), sizeof(BYTE) * 3);

                            indexInfo.vecPmxVertexIndex_us16.push_back(pmxVertexIndex_us16);
                        }
                        else if (header.vertexIndexSize == 2) 
                        {
                            ifs.read(reinterpret_cast<char*>(&pmxVertexIndex_us16.indices), sizeof(USHORT) * 3);

                            indexInfo.vecPmxVertexIndex_us16.push_back(pmxVertexIndex_us16);
                        }
                        else if (header.vertexIndexSize == 4) 
                        {
                            ifs.read(reinterpret_cast<char*>(&pmxVertexIndex_ui32.indices), sizeof(UINT32) * 3);

                            indexInfo.vecPmxVertexIndex_ui32.push_back(pmxVertexIndex_ui32);
                        }
                    }
                };

            LoadIndices();
        }

        PmxTextureInfo textureInfo;
        {
            ifs.read(reinterpret_cast<char*>(&textureInfo.pmxTextureCount), sizeof(PmxTextureInfo::pmxTextureCount));

            auto LoadTextureFilePaths = [&]()
                {
                    int textureCount = textureInfo.pmxTextureCount;

                    for (int i = 0; i < textureCount; i++) 
                    {
                        PmxTextBuffer pmxTextureBuffer;

                        getTextBuffer(pmxTextureBuffer);

                        auto parentPath = std::filesystem::path(filePath).parent_path().string();

                        pmxTextureBuffer.pmxInfoStr = parentPath + "/textures/" + pmxTextureBuffer.pmxInfoStr;

                        textureInfo.vecPmxTextureFileName.push_back(pmxTextureBuffer);
                    }
                };

            LoadTextureFilePaths();
        }

        PmxMaterialInfo materialInfo;
        {
            ifs.read(reinterpret_cast<char*>(&materialInfo.materialCount), sizeof(PmxMaterialInfo::materialCount));

            auto LoadMaterials = [&]()
                {
                    int materialCount = materialInfo.materialCount;
                    for (int i = 0; i < materialCount; i++) 
                    {
                        PmxMaterial pmxMaterial;

                        getTextBuffer(pmxMaterial.materialNameJa);
                        getTextBuffer(pmxMaterial.materialNameEn);

                        ifs.read(reinterpret_cast<char*>(&pmxMaterial.materialDiffuse), sizeof(PmxMaterial::materialDiffuse));
                        ifs.read(reinterpret_cast<char*>(&pmxMaterial.materialSpecular), sizeof(PmxMaterial::materialSpecular));
                        ifs.read(reinterpret_cast<char*>(&pmxMaterial.materialSpecularPower), sizeof(PmxMaterial::materialSpecularPower));
                        ifs.read(reinterpret_cast<char*>(&pmxMaterial.materialAmbient), sizeof(PmxMaterial::materialAmbient));
                        ifs.read(reinterpret_cast<char*>(&pmxMaterial.bitFlag), sizeof(PmxMaterial::bitFlag));
                        ifs.read(reinterpret_cast<char*>(&pmxMaterial.materialEdge), sizeof(PmxMaterial::materialEdge));
                        ifs.read(reinterpret_cast<char*>(&pmxMaterial.edgeSize), sizeof(PmxMaterial::edgeSize));
                        ifs.read(reinterpret_cast<char*>(&pmxMaterial.diffuseTextureIndex), sizeof(PmxMaterial::diffuseTextureIndex));
                        ifs.read(reinterpret_cast<char*>(&pmxMaterial.sphereTextureIndex), sizeof(PmxMaterial::sphereTextureIndex));
                        ifs.read(reinterpret_cast<char*>(&pmxMaterial.sphereMode), sizeof(PmxMaterial::sphereMode));
                        ifs.read(reinterpret_cast<char*>(&pmxMaterial.shareToonFlag), sizeof(PmxMaterial::shareToonFlag));

                        if (pmxMaterial.shareToonFlag == 0) 
                            ifs.read(reinterpret_cast<char*>(&pmxMaterial.toonTextureIndex), sizeof(PmxMaterial::toonTextureIndex));
                        else if (pmxMaterial.shareToonFlag == 1) 
                            ifs.read(reinterpret_cast<char*>(&pmxMaterial.shareToonTextureIndex), sizeof(PmxMaterial::shareToonTextureIndex));

                        getTextBuffer(pmxMaterial.memo);

                        ifs.read(reinterpret_cast<char*>(&pmxMaterial.materialPolygonCount), sizeof(PmxMaterial::materialPolygonCount));
                        pmxMaterial.materialPolygonCount /= 3;

                        materialInfo.vecPmxMaterial.push_back(pmxMaterial);
                    }
                };

            LoadMaterials();
        }

        {
            int vertexCount = static_cast<int>(vertexInfo.vecPmxVertex.size());

            std::vector<VertexBuffer> vertices;
            for (auto i = 0; i < vertexCount; ++i)
            {
                VertexBuffer vertexBuffer;

                vertexBuffer.vertex = vertexInfo.vecPmxVertex[i].vertex;
                vertexBuffer.normal = vertexInfo.vecPmxVertex[i].normal;
                vertexBuffer.uv = vertexInfo.vecPmxVertex[i].uv;

                vertices.push_back(vertexBuffer);
            }

            std::map<UINT, std::unique_ptr<Texture>> textureMap;
            for (auto i = 0; i < textureInfo.vecPmxTextureFileName.size(); ++i)
                textureMap[i] = std::move(std::make_unique<Texture>());

            UINT startIndex = 0;
            UINT endIndex = 0;

            for (const auto& pmxMaterial : materialInfo.vecPmxMaterial)
            {
                endIndex +=  pmxMaterial.materialPolygonCount;

                std::vector<DWORD> indices;
                if (!indexInfo.vecPmxVertexIndex_us16.empty())
                {
                    for (auto i = startIndex; i < endIndex; ++i)
                    {
                        indices.push_back(indexInfo.vecPmxVertexIndex_us16[i].indices[0]);
                        indices.push_back(indexInfo.vecPmxVertexIndex_us16[i].indices[1]);
                        indices.push_back(indexInfo.vecPmxVertexIndex_us16[i].indices[2]);
                    }
                }
                else if (!indexInfo.vecPmxVertexIndex_ui32.empty())
                {
                    for (auto i = startIndex; i < endIndex; ++i)
                    {
                        indices.push_back(indexInfo.vecPmxVertexIndex_ui32[i].indices[0]);
                        indices.push_back(indexInfo.vecPmxVertexIndex_ui32[i].indices[1]);
                        indices.push_back(indexInfo.vecPmxVertexIndex_ui32[i].indices[2]);
                    }
                }

                startIndex = endIndex;

                // マテリアル毎にどのメッシュを使用するのか決まっているのでそこでメッシュも作成するようにしている
                auto pMesh = std::make_unique<Mesh>();
                pMesh->Initialize(vertices, indices);

                ModelDrawInfoParam param;
                param.pMesh = pMesh.get();

                MaterialBuffer materialBuffer;
                materialBuffer.diffuse = pmxMaterial.materialDiffuse;
                materialBuffer.specular = VECTOR4(pmxMaterial.materialSpecular.x, pmxMaterial.materialSpecular.y, pmxMaterial.materialSpecular.z, pmxMaterial.materialSpecularPower);
                materialBuffer.ambient = VECTOR4(pmxMaterial.materialAmbient.x, pmxMaterial.materialAmbient.y, pmxMaterial.materialAmbient.z, 1.0f);

                auto pMaterial = std::make_unique<Material>();
                pMaterial->Initialize(materialBuffer);

                auto itr = textureMap.find(pmxMaterial.diffuseTextureIndex);
                if (itr != textureMap.cend())
                {
                    std::vector<Texture*> vecTextures;
                    vecTextures.push_back(itr->second.get());

                    param.pMaterialTexturesMap[pMaterial.get()] = vecTextures;
                }

                pModelMesh->RegisterMaterial(pMaterial);
                pModelMesh->RegisterMesh(pMesh);

                pModelMesh->AddModelDrawInfoParam(param);
            }

            for(auto itr = textureMap.begin(); itr != textureMap.end(); ++itr)
            {
                auto filePath = textureInfo.vecPmxTextureFileName[itr->first].pmxInfoStr;
                pModelMesh->RegisterTexture(filePath, itr->second);
            }
        }

        return true;
    }

} // namespace ModelLoader
} // namespace Simple