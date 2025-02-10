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
    class ModelMesh;
    class Material;

    struct VertexBuffer;
    struct ModelDrawInfoParam;

namespace ModelLoader
{
    class FBXLoader : public IModelLoader
    {
    private:
        FBXLoader(const FBXLoader&) = delete;
        FBXLoader operator=(const FBXLoader&) = delete;

    private:
        void ConvertMesh(FbxMesh* pFbxMesh, Simple::ModelMesh* pModelMesh, ModelDrawInfoParam* pParam);
        void ConvertVertex(FbxMesh* pFbxMesh, std::vector<Simple::VertexBuffer>& vertices, std::vector<DWORD>& indices);
        void ConvertNormal(FbxMesh* pFbxMesh, std::vector<Simple::VertexBuffer>& vertices, const std::vector<DWORD>& indices, std::vector<VECTOR3> vecNormal);
        void ConvertColor(FbxMesh* pFbxMesh, std::vector<Simple::VertexBuffer>& vertices, const std::vector<DWORD>& indices);
        void ConvertUV(FbxMesh* pFbxMesh, std::vector<Simple::VertexBuffer>& vertices, const std::vector<DWORD>& indices, std::vector<VECTOR2>& vecUV);

        void ConvertMaterial(FbxSurfaceMaterial* pFbxMaterial, Simple::ModelMesh* pModelMesh, ModelDrawInfoParam* pParam);

        void RecursiveFbxNode(FbxNode* pRootNode, std::vector<FbxNode*>& outFbxNodes);
    public:
        FBXLoader();
        ~FBXLoader();

        bool LoadModel(const std::string& filePath, Simple::ModelMesh* pModelMesh) override;
    };
} // namespace ModelLoader
} // namespacce Simple

#endif
