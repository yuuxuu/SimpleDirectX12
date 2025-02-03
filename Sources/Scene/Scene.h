/**
 * @file Scene.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/19
 */

#pragma once

#ifndef _SCENE_H_
#define _SCENE_H_

#include "GraphicsAPI/IGraphics.h"

namespace Simple
{
    class Camera;
    class Mesh;

    using MeshVec = std::vector<std::unique_ptr<Simple::Mesh>>;

    class Scene
    {
    private:
        std::unique_ptr<Camera>                 m_pCamera;

        MeshVec                                 m_pMeshVec;

        Graphics::IGraphics*                    pGraphics;

    private:
        Scene(const Scene&) = delete;
        Scene operator=(const Scene&) = delete;

    public:
        explicit Scene(Graphics::IGraphics* graphics);
        ~Scene();

        void SetUpScene(const UINT windowWidth, const UINT windowHeight);

        void UpdateScene(const UINT windowWidth, const UINT windowHeight);
    };
} // namespace Simple

#endif // _SCENE_H_
