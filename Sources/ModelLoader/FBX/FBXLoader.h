/**
 * @file FBXLoader.h
 * @brief
 * @author Yu Kimura
 * @date 2021/11/07
 */

#pragma once

#ifndef _FBX_LOADER_H_
#define _FBX_LOADER_H_

#include "math/math.h"

#include "ModelLoader/IModelLoader.h"

namespace Simple 
{
    class Mesh;
    class MeshBone;
    class ModelMesh;
    class Material;
    class Texture;

    struct VertexBuffer;
    struct ModelDrawInfoParam;

    using RegisterMaterialMap = std::map<std::string, std::unique_ptr<Simple::Material>>;
    using RegisterTextureMap = std::map<std::string, std::unique_ptr<Simple::Texture>>;

namespace ModelLoader
{
    class FBXLoader : public IModelLoader
    {
    private:
        FBXLoader(const FBXLoader&) = delete;
        FBXLoader operator=(const FBXLoader&) = delete;

    private:
        void ConvertMesh(FbxMesh* pFbxMesh, ModelMesh* pModelMesh, ModelDrawInfoParam* pParam);
        void ConvertVertex(FbxMesh* pFbxMesh, std::vector<VertexBuffer>& vertices, std::vector<DWORD>& indices);
        void ConvertNormal(FbxMesh* pFbxMesh, std::vector<VertexBuffer>& vertices, const std::vector<DWORD>& indices, std::vector<VECTOR3> vecNormal);
        void ConvertColor(FbxMesh* pFbxMesh, std::vector<VertexBuffer>& vertices, const std::vector<DWORD>& indices);
        void ConvertUV(FbxMesh* pFbxMesh, std::vector<VertexBuffer>& vertices, const std::vector<DWORD>& indices, std::vector<VECTOR2>& vecUV);
        void ConvertMeshIndexWeight(FbxMesh* pFbxMesh, std::vector<VertexBuffer>& vertices, const std::vector<DWORD>& indices);

        void ConvertMeshBone(FbxNode* pFbxNode, ModelMesh* pModelMesh);
        
        void ConvertMaterial(FbxSurfaceMaterial* pFbxMaterial, ModelDrawInfoParam* pParam, RegisterMaterialMap& materialMap, RegisterTextureMap& textureMap);

        void RecursiveFbxNode(FbxNode* pRootNode, std::vector<FbxNode*>& outFbxNodes);
    public:
        FBXLoader();
        ~FBXLoader();

        bool LoadModel(const std::string& filePath, ModelMesh* pModelMesh) override;
    };
} // namespace ModelLoader
} // namespacce Simple

#endif
