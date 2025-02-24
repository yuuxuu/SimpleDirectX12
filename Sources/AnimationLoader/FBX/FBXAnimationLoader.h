/**
 * @file FBXAnimationLoader.h
 * @brief
 * @author Yu Kimura
 * @date 2021/11/07
 */

#pragma once

#ifndef _FBX_ANIMATION_LOADER_H_
#define _FBX_ANIMATION_LOADER_H_

#include "math/math.h"

#include "AnimationLoader/IAnimationLoader.h"

namespace Simple 
{
    class MeshAnimation;

namespace AnimationLoader
{
    using AnimationBoneMatrixMap = std::map<std::string, std::vector<Matrix>>;

    class FBXAnimationLoader : public IAnimationLoader
    {
    private:
        FBXAnimationLoader(const FBXAnimationLoader&) = delete;
        FBXAnimationLoader operator=(const FBXAnimationLoader&) = delete;

        void ConvertMeshAnimation(const std::vector<FbxNode*>& vecFbxNode, const std::vector<FbxLongLong>& vecFrame, AnimationBoneMatrixMap& animationBoneMatrixMap, MeshAnimation* pMeshAnimation);

        void RecursiveFbxNode(FbxNode* pRootNode, std::vector<FbxNode*>& outFbxNodes);

    public:
        FBXAnimationLoader();
        ~FBXAnimationLoader();

        bool LoadAnimation(const std::string& filePath, Simple::ModelMesh* pModelMesh) override;
    };
} // namespace ModelLoader
} // namespace Simple

#endif // _FBX_ANIMATION_LOADER_H_
