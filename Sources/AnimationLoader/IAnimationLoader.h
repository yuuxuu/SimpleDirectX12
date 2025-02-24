/**
 * @file IAnimationLoader.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/09
 */

#pragma once

#ifndef _IANIMATION_LOADER_H_
#define _IANIMATION_LOADER_H_

namespace Simple 
{
    class ModelMesh;

namespace AnimationLoader
{
    class IAnimationLoader
    {
    public:
        virtual bool LoadAnimation(const std::string& filePath, Simple::ModelMesh* pModelMesh) = 0;
    };
} // namespace AnimationLoader
} // namespace Simple

#endif // _IANIMATION_LOADER_H_
