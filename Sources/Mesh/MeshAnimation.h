/**
 * @file MeshAnimation.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/19
 */

#pragma once

#ifndef _MESH_ANIMATION_H_
#define _MESH_ANIMATION_H_

#include "Math/math.h"

namespace Simple 
{
    using MeshAnimationMatrixMap = std::map<std::string, std::vector<Matrix>>;

    class MeshAnimation 
    {
    private:
        UINT                    m_startFrame;
        UINT                    m_stopFrame;
        UINT                    m_nowFrame;

        Matrix                  m_identityMatrix;
        MeshAnimationMatrixMap  m_initMeshAnimationMatrixMap;
        MeshAnimationMatrixMap  m_MeshAnimationMatrixMap;

    private:
        MeshAnimation(const MeshAnimation&) = delete;
        MeshAnimation operator=(const MeshAnimation&) = delete;

        const bool IsEndAnimation(UINT frame) const;
    public:
        explicit MeshAnimation(UINT startFrame, UINT stopFrame);
        ~MeshAnimation();

        void Initialize(const MeshAnimationMatrixMap& mapAnimationMatrixMap);

        void UpdateFrame();

        const Matrix& GetInitMeshAnimationMatrix(const std::string& boneName) const;

        const Matrix& GetMeshAnimationMatrix(const std::string& boneName) const;

        const UINT GetFrameCount() const;
    };

} // namespace Simple

#endif // _MESH_ANIMATION_H_
