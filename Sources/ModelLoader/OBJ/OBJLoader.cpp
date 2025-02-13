/**
 * @file OBJLoader.cpp
 * @brief
 * @author 木村優
 * @date 2025/02/09
 */

#include "OBJLoader.h"

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
    OBJLoader::OBJLoader()
    {}

    // デストラクタ
    OBJLoader::~OBJLoader()
    {}

    // モデルの読み込み
    bool OBJLoader::LoadModel(const std::string& filePath, Simple::ModelMesh* pModelMesh) 
    {
        if (!LoadMesh(filePath, pModelMesh)) return false;

        return true;
    }

    bool OBJLoader::LoadMesh(const std::string& filePath, Simple::ModelMesh* pModelMesh) 
    {
        std::ifstream ifs;
        ifs.open(filePath);
        if (!ifs)
            return false;

        std::vector<VECTOR3> vertices;
        std::vector<VECTOR3> normals;
        std::vector<VECTOR2> uvs;

        std::vector<DWORD> indices;
        std::vector<DWORD> normalIndices;
        std::vector<DWORD> uvIndices;

        std::map<std::string, ModelDrawInfoParam> modelDrawInfoParamMap;
        std::string materialName;

        std::map<std::string, std::function<void(const std::string&, const UINT)>> findKeyActionMap;
        findKeyActionMap["mtllib "] = [&](const std::string& lineString, const UINT pos)
            {
                auto mtllibName = lineString.substr(pos, lineString.size() - pos);

                const auto charSize = 128;

                char drive[charSize];
                char dir[charSize];
                char name[charSize];
                char extension[charSize];

                _splitpath_s(filePath.c_str(), drive, dir, name, extension);

                auto path = std::string(drive) + std::string(dir) + std::string(mtllibName);

                if (!LoadMaterial(path, modelDrawInfoParamMap, pModelMesh))
                    return false;

                return true;
            };
        findKeyActionMap["usemtl "] = [&](const std::string& lineString, const UINT pos)
            {
                auto numIndex = indices.size();
                if (numIndex > 0)
                {
                    std::vector<VertexBuffer> vecVertexBuffer(numIndex);
                    for (auto i = 0; i < numIndex; ++i)
                    {
                        if (!indices.empty())
                            vecVertexBuffer[i].vertex = vertices[indices[i]];
                        if(!normalIndices.empty())
                            vecVertexBuffer[i].normal = normals[normalIndices[i]];
                        if (!uvIndices.empty())
                            vecVertexBuffer[i].uv = uvs[uvIndices[i]];
                    }

                    indices.clear();
                    normalIndices.clear();
                    uvIndices.clear();

                    auto pMesh = std::make_unique<Mesh>();

                    pMesh->Initialize(vecVertexBuffer, std::vector<DWORD>());

                    auto itr = modelDrawInfoParamMap.find(materialName);
                    if (itr != modelDrawInfoParamMap.cend())
                    {
                        itr->second.pMesh = pMesh.get();

                        pModelMesh->AddModelDrawInfoParam(itr->second);
                    }

                    pModelMesh->RegisterMesh(pMesh);
                }

                materialName = lineString.substr(pos, lineString.size() - pos);
            };
        findKeyActionMap["v "] = [&](const std::string& lineString, const UINT pos)
            {
                std::stringstream ss(lineString.substr(pos));

                VECTOR3 vertex;
                ss >> vertex.x >> vertex.y >> vertex.z;

                vertices.emplace_back(vertex);
            };
        findKeyActionMap["vn "] = [&](const std::string& lineString, const UINT pos)
            {
                std::stringstream ss(lineString.substr(pos));

                VECTOR3 normal;
                ss >> normal.x >> normal.y >> normal.z;

                normals.emplace_back(normal);
            };
        findKeyActionMap["vt "] = [&](const std::string& lineString, const UINT pos)
            {
                std::stringstream ss(lineString.substr(pos));

                VECTOR2 uv;
                ss >> uv.x >> uv.y;

                uvs.emplace_back(uv);
            };
        findKeyActionMap["f "] = [&](const std::string& lineString, const UINT pos)
            {
                std::vector<DWORD> vecVertexIndex;
                std::vector<DWORD> vecNormalIndex;
                std::vector<DWORD> vecUVIndex;

                std::stringstream ss(lineString.substr(pos));

                std::string bufString;
                const int typeCount = 3;
                while (std::getline(ss, bufString, ' '))
                {
                    if (bufString.empty()) 
                        continue;

                    bufString += "/";

                    int indedxType[typeCount] = { -1, -1, -1 };

                    auto numberCnt = 0;
                    auto size = static_cast<int>(bufString.size());
                    auto start = 0;
                    auto end = 0;
                    for (auto i = 0; i < size; ++i)
                    {
                        auto numberString = bufString.substr(i, 1);
                        auto nextNumberStr = bufString.substr(i + 1, 1);
                        if (numberString == "/" && nextNumberStr == "/")
                        {
                            numberCnt++;
                            i++;
                            continue;
                        }
                        else if (numberString == "/")
                        {
                            end = i;

                            auto str = bufString.substr(start, end - start);

                            indedxType[numberCnt++] = std::stoi(str) - 1;

                            start = i + 1;
                        }
                    }
                    for (auto i = 0; i < typeCount; ++i)
                    {
                        if (indedxType[i] < 0)
                            continue;
                        if (i == 0)
                            vecVertexIndex.push_back(indedxType[i]);
                        else if (i == 1)
                            vecUVIndex.push_back(indedxType[i]);
                        else if (i == 2)
                            vecNormalIndex.push_back(indedxType[i]);
                    }
                }

                auto vertexCnt = static_cast<int>(vecVertexIndex.size());
                if (vertexCnt == 3)
                {
                    for (auto i = 0; i < vertexCnt; ++i)
                    {
                        if (!vecVertexIndex.empty())
                            indices.push_back(vecVertexIndex[i]);
                        if (!vecNormalIndex.empty())
                            normalIndices.push_back(vecNormalIndex[i]);
                        if (!vecUVIndex.empty())
                            uvIndices.push_back(vecUVIndex[i]);
                    }
                }
                else if (vertexCnt == 4)
                {
                    static const int indexOrder[] =
                    {
                        0, 1, 2,
                        0, 2, 3
                    };
                    for (auto index : indexOrder)
                    {
                        if (!vecVertexIndex.empty())
                            indices.push_back(vecVertexIndex[index]);
                        if (!vecNormalIndex.empty())
                            normalIndices.push_back(vecNormalIndex[index]);
                        if (!vecUVIndex.empty())
                            uvIndices.push_back(vecUVIndex[index]);
                    }
                }
            };

        std::string lineString;
        while (!ifs.eof()) 
        {
            std::getline(ifs, lineString);
            if (lineString.empty()) 
                continue;
            if (lineString.find("#") != std::string::npos)
                continue;

            for (const auto& itr : findKeyActionMap)
            {
                auto pos = lineString.find(itr.first);
                if (pos == std::string::npos)
                    continue;

                itr.second(lineString, (UINT)itr.first.size());
            }
        }

        auto numIndex = indices.size();
        if (numIndex > 0)
        {
            std::vector<VertexBuffer> vecVertexBuffer(numIndex);
            for (auto i = 0; i < numIndex; ++i)
            {
                if (!indices.empty())
                    vecVertexBuffer[i].vertex = vertices[indices[i]];
                if (!normalIndices.empty())
                    vecVertexBuffer[i].normal = normals[normalIndices[i]];
                if (!uvIndices.empty())
                    vecVertexBuffer[i].uv = uvs[uvIndices[i]];
            }

            auto pMesh = std::make_unique<Mesh>();
            pMesh->Initialize(vecVertexBuffer, std::vector<DWORD>());

            auto itr = modelDrawInfoParamMap.find(materialName);
            if (itr != modelDrawInfoParamMap.cend())
            {
                itr->second.pMesh = pMesh.get();

                pModelMesh->AddModelDrawInfoParam(itr->second);
            }

            pModelMesh->RegisterMesh(pMesh);
        }

        return true;
    }

    bool OBJLoader::LoadMaterial(const std::string& filePath, std::map<std::string, ModelDrawInfoParam>& modelDrawInfoParamMap, Simple::ModelMesh* pModelMesh)
    {
        std::ifstream ifs;
        ifs.open(filePath);
        if (!ifs) return false;

        std::string materialName;
        std::unique_ptr<Material> pMaterial;
        MaterialBuffer materialBuffer;

        std::map<std::string, std::function<void(const std::string&, const UINT)>> findKeyActionMap;
        findKeyActionMap["newmtl "] = [&](const std::string& lineString, const UINT pos)
            {
                if (pMaterial)
                {
                    pMaterial->Initialize(materialBuffer);

                    auto itr = modelDrawInfoParamMap.find(materialName);
                    if (itr != modelDrawInfoParamMap.cend())
                        itr->second.pMaterialTexturesMap[pMaterial.get()];

                    pModelMesh->RegisterMaterial(pMaterial);
                }

                materialName = lineString.substr(pos, lineString.size() - pos);

                if (!pMaterial)
                {
                    modelDrawInfoParamMap[materialName];

                    pMaterial = std::make_unique<Material>();
                }
            };
        findKeyActionMap["Ka "] = [&](const std::string& lineString, const UINT pos)
            {
                std::stringstream ss(lineString.substr(pos));

                ss >> materialBuffer.ambient.x >> materialBuffer.ambient.y >> materialBuffer.ambient.z;
                materialBuffer.ambient.w = 1.0f;
            };
        findKeyActionMap["Kd "] = [&](const std::string& lineString, const UINT pos)
            {
                std::stringstream ss(lineString.substr(pos));

                VECTOR4 diffuse;
                ss >> materialBuffer.diffuse.x >> materialBuffer.diffuse.y >> materialBuffer.diffuse.z;
                materialBuffer.diffuse.w = 1.0f;
            };
        findKeyActionMap["Ks "] = [&](const std::string& lineString, const UINT pos)
            {
                std::stringstream ss(lineString.substr(pos));

                VECTOR4 specular;
                ss >> materialBuffer.specular.x >> materialBuffer.specular.y >> materialBuffer.specular.z;
                specular.w = 1.0f;
            };
        findKeyActionMap["map_Kd "] = [&](const std::string& lineString, const UINT pos)
            {
                std::stringstream ss(lineString.substr(pos));

                std::string textureFileName;
                while (std::getline(ss, textureFileName, '\\')) {}

                ss = std::stringstream(textureFileName);
                while (std::getline(ss, textureFileName, ' ')) {}

                const auto charSize = 128;

                char drive[charSize];
                char dir[charSize];
                char name[charSize];
                char extension[charSize];

                _splitpath_s(filePath.c_str(), drive, dir, name, extension);

                auto path = std::string(drive) + std::string(dir) + "textures/" + std::string(textureFileName);

                auto pTexture = std::make_unique<Texture>();

                auto itr = modelDrawInfoParamMap.find(materialName);
                if (itr != modelDrawInfoParamMap.cend())
                    itr->second.pMaterialTexturesMap[pMaterial.get()].push_back(pTexture.get());

                pModelMesh->RegisterTexture(path, pTexture);
            };

        std::string lineString;
        while (!ifs.eof())
        {
            std::getline(ifs, lineString);
            if (lineString.empty())
                continue;
            if (lineString.find("#") != std::string::npos)
                continue;

            for (const auto& itr : findKeyActionMap)
            {
                auto pos = lineString.find(itr.first);
                if (pos == std::string::npos)
                    continue;

                itr.second(lineString, (UINT)itr.first.size());
            }
        }

        if (pMaterial)
        {
            pMaterial->Initialize(materialBuffer);

            auto itr = modelDrawInfoParamMap.find(materialName);
            if (itr != modelDrawInfoParamMap.cend())
                itr->second.pMaterialTexturesMap[pMaterial.get()];

            pModelMesh->RegisterMaterial(pMaterial);
        }

        return true;
    }
} // namespace ModelLoader
} // namespace Simple