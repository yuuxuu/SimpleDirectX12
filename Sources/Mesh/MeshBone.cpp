/**
 * @file MeshBone.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/02/17
 */

#include "MeshBone.h"

namespace Simple {

    // コンストラクタ
    MeshBone::MeshBone(const std::string boneName,const Matrix& boneMatrix) :
        m_initBoneMatrix(boneMatrix),
        m_boneName(boneName)
    {}

    // デストラクタ
    MeshBone::~MeshBone()
    {}

    const Matrix& MeshBone::GetInitBoneMatrix() const
    {
        return m_initBoneMatrix;
    }

    const std::string& MeshBone::GetBoneName() const
    {
        return m_boneName;
    }

} // namespace Simple