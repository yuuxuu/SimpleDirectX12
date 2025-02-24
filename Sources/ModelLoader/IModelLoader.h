/**
 * @file IModelLoader.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/09
 */

#pragma once

#ifndef _IMODEL_LOADER_H_
#define _IMODEL_LOADER_H_

namespace Simple 
{
    class ModelMesh;

namespace ModelLoader
{
    class IModelLoader
    {
    public:
        virtual bool LoadModel(const std::string& filePath, Simple::ModelMesh* pModelMesh) = 0;
    };
} // namespace ModelLoader
} // namespace Simple

#endif // _IMODEL_LOADER_H_
