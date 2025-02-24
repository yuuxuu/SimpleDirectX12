/**
 * @file MeshAnimation.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/02/19
 */

#include "MeshAnimation.h"

namespace Simple 
{
    MeshAnimation::MeshAnimation(UINT startFrame, UINT stopFrame) :
        m_startFrame(startFrame),
        m_stopFrame(stopFrame),
        m_nowFrame(0),
        m_identityMatrix()
    {}

    MeshAnimation::~MeshAnimation()
    {}

    const bool MeshAnimation::IsEndAnimation(UINT frame) const
    {
        return frame > m_stopFrame;
    }

    void MeshAnimation::Initialize(const MeshAnimationMatrixMap& mapAnimationMatrix)
    {
        m_MeshAnimationMatrixMap = mapAnimationMatrix;
    }

    void MeshAnimation::UpdateFrame()
    {
        m_nowFrame++;

        if (IsEndAnimation(m_nowFrame))
            m_nowFrame = m_startFrame;
    }

    const Matrix& MeshAnimation::GetInitMeshAnimationMatrix(const std::string& boneName) const
    {
        auto itr = m_MeshAnimationMatrixMap.find(boneName);
        if (itr == m_MeshAnimationMatrixMap.cend())
            return m_identityMatrix;

        return itr->second.at(0);
    }

    const Matrix& MeshAnimation::GetMeshAnimationMatrix(const std::string& boneName) const
    {
        auto itr = m_MeshAnimationMatrixMap.find(boneName);
        if (itr == m_MeshAnimationMatrixMap.cend())
            return m_identityMatrix;

        return itr->second.at(m_nowFrame);
    }

    const UINT MeshAnimation::GetFrameCount() const
    {
        return m_stopFrame - m_startFrame;
    }

} // namespace Simple