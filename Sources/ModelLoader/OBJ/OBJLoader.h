/**
 * @file OBJLoader.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/09
 */

#pragma once

#ifndef _OBJ_LOADER_H_
#define _OBJ_LOADER_H_

#include "ModelLoader/IModelLoader.h"

namespace Simple 
{
    class Mesh;
    class ModelMesh;
    class Material;
    class Texture;

    struct VertexBuffer;
    struct ModelDrawInfoParam;

    using RegisterMaterialMap = std::map<std::string, std::unique_ptr<Simple::Material>>;
    using RegisterTextureMap = std::map<std::string, std::unique_ptr<Simple::Texture>>;

namespace ModelLoader
{
    class OBJLoader : public IModelLoader
    {
    private:
        OBJLoader(const OBJLoader&) = delete;
        OBJLoader operator=(const OBJLoader&) = delete;

    private:
        bool LoadMesh(const std::string& filePath, Simple::ModelMesh* pModelMesh);
        bool LoadMaterial(const std::string& filePath, RegisterMaterialMap& materialMap, RegisterTextureMap& textureMap, Simple::ModelMesh* pModelMesh);

    public:
        OBJLoader();
        ~OBJLoader();

        bool LoadModel(const std::string& filePath, Simple::ModelMesh* pModelMesh) override;
    };
} // namespace ModelLoader
} // namespace Simple

#endif // _OBJ_LOADER_H_
