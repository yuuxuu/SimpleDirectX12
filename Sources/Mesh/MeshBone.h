/**
 * @file MeshBone.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/17
 */

#pragma once

#ifndef _MESH_BONE_H_
#define _MESH_BONE_H_

#include "Math/math.h"

namespace Simple 
{
    class MeshBone 
    {
    private:
        Matrix      m_initBoneMatrix;

        std::string m_boneName;

    private:
        MeshBone(const MeshBone&) = delete;
        MeshBone operator=(const MeshBone&) = delete;

    public:
        explicit MeshBone(const std::string boneName, const Matrix& boneMatrix);
        ~MeshBone();

        const Matrix& GetInitBoneMatrix() const;

        const std::string& GetBoneName() const;
    };

} // namespace Simple

#endif // _MESH_BONE_H_
