/**
 * @file FBXAnimationLoader.cpp
 * @brief
 * @author 木村優
 * @date 2021/11/07
 */

#include "FBXAnimationLoader.h"

#include "math/math.h"

#include "Param/ModelDrawInfoParam.h"

#include "Mesh/ModelMesh.h"
#include "Mesh/MeshAnimation.h"

namespace Simple 
{
namespace AnimationLoader
{
    // コンストラクタ
    FBXAnimationLoader::FBXAnimationLoader()
    {}

    // デストラクタ
    FBXAnimationLoader::~FBXAnimationLoader()
    {}

    void FBXAnimationLoader::ConvertMeshAnimation(
        const std::vector<FbxNode*>& vecFbxNode, 
        const std::vector<FbxLongLong>& vecFrame, 
        AnimationBoneMatrixMap& animationBoneMatrixMap,
        MeshAnimation* pMeshAnimation)
    {
        for (const auto pFbxNode : vecFbxNode)
        {
            auto pFbxSkelton = pFbxNode->GetSkeleton();
            if (!pFbxSkelton)
                continue;

            std::vector<Matrix> vecMeshAnimationMatix;

            for (const auto frame : vecFrame)
            {
                const auto& fbxMatrix = pFbxNode->EvaluateGlobalTransform(frame);

                auto row = 4;
                auto col = 4;
                Matrix matrix;
                for (int i = 0; i < row; i++)
                    for (int j = 0; j < col; j++)
                        matrix.m[i][j] = static_cast<float>(fbxMatrix.Get(i, j));

                vecMeshAnimationMatix.emplace_back(matrix);
            }

            auto boneName = pFbxNode->GetName();
            animationBoneMatrixMap[boneName] = vecMeshAnimationMatix;
        }

        pMeshAnimation->Initialize(animationBoneMatrixMap);
    }

    void FBXAnimationLoader::RecursiveFbxNode(FbxNode* pRootNode, std::vector<FbxNode*>& outFbxNodes)
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

            if (pFbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
            {
                auto pFbxSkelton = pChildNode->GetSkeleton();
                if (!pFbxSkelton)
                    continue;

                outFbxNodes.emplace_back(pChildNode);
            }

            RecursiveFbxNode(pChildNode, outFbxNodes);
        }
    }

    bool FBXAnimationLoader::LoadAnimation(const std::string& filePath, Simple::ModelMesh* pModelMesh)
    {
        auto pFbxManager = FbxManager::Create();

        auto pFbxInporter = FbxImporter::Create(pFbxManager, "");
        if (!pFbxInporter->Initialize(filePath.c_str(), -1, pFbxManager->GetIOSettings())) {
            std::cout << filePath << "= 読み込み：失敗" << std::endl;
            return false;
        }

        auto pFbxScene = FbxScene::Create(pFbxManager, "");
        pFbxInporter->Import(pFbxScene);

        auto pRootNode = pFbxScene->GetRootNode();
        if (!pRootNode) 
            return false;

        std::vector<FbxNode*> vecFbxNode;
        RecursiveFbxNode(pRootNode, vecFbxNode);

        if (vecFbxNode.empty())
            return false;

        auto animStackCount = pFbxInporter->GetAnimStackCount();
        if (animStackCount <= 0)
            return false;

        auto pFbxTakeInfo = pFbxInporter->GetTakeInfo(0);
        if (!pFbxTakeInfo)
            return false;

        auto startTime = pFbxTakeInfo->mLocalTimeSpan.GetStart();
        auto stopTime = pFbxTakeInfo->mLocalTimeSpan.GetStop();
        
        const auto& fbxGlobalSetting = pFbxScene->GetGlobalSettings();
        auto timeMode = fbxGlobalSetting.GetTimeMode();

        FbxTime framePeriod;
        framePeriod.SetTime(0, 0, 0, 1, 0, timeMode);

        auto startFrame = static_cast<int>(startTime.Get() / framePeriod.Get());
        auto stopFrame = static_cast<int>(stopTime.Get() / framePeriod.Get());
        auto frameCount = stopFrame - startFrame;

        std::vector<FbxLongLong> vecFrame;
        for (auto i = 0; i <= frameCount; ++i)
        {
            auto frame = startTime.Get() + framePeriod.Get() * i;

            vecFrame.emplace_back(frame);
        }

        if (startFrame < 0)
            startFrame = 0;

        auto pMeshAnimation = std::make_unique<MeshAnimation>(startFrame, stopFrame);

        AnimationBoneMatrixMap animationBoneMatrixMap;
        ConvertMeshAnimation(vecFbxNode, vecFrame, animationBoneMatrixMap, pMeshAnimation.get());

        if (animationBoneMatrixMap.empty())
            return false;

        pModelMesh->RegisterMeshAnimation(pRootNode->GetName(), pMeshAnimation);

        pFbxInporter->Destroy();

        return true;
    }
} // namespace AnimationLoader
} // namespace Simple
