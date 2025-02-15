/**
 * @file PMXLoader.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/14
 */

#pragma once

#ifndef _PMX_LOADER_H_
#define _PMX_LOADER_H_

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
    class PMXLoader : public IModelLoader
    {
    private:
        PMXLoader(const PMXLoader&) = delete;
        PMXLoader operator=(const PMXLoader&) = delete;

    public:
        PMXLoader();
        ~PMXLoader();

        bool LoadModel(const std::string& filePath, Simple::ModelMesh* pModelMesh) override;
    };
} // namespace ModelLoader
} // namespace Simple

#endif // _PMX_LOADER_H_
