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
    class ModelMesh;

    using ModelMeshVec = std::vector<std::unique_ptr<Simple::ModelMesh>>;

    class Scene
    {
    private:
        ModelMeshVec                            m_pModelMeshVec;

    private:
        Scene(const Scene&) = delete;
        Scene operator=(const Scene&) = delete;

    public:
        Scene();
        ~Scene();

        void SetUpScene(Graphics::IGraphics* graphics, const UINT windowWidth, const UINT windowHeight);

        void UpdateScene(Graphics::IGraphics* graphics);
    };
} // namespace Simple

#endif // _SCENE_H_
