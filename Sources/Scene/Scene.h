/**
 * @file Scene.h
 * @brief
 * @author Yu Kimura
 * @date 2025/01/19
 */

#pragma once

#ifndef _SCENE_H_
#define _SCENE_H_

#include "GraphicsAPI/DirectX12/IGraphics.h"

namespace Simple
{
    namespace Light
    {
        class DirectionalLight;
        class PointLight;
    }

    class ModelMesh;

    using ModelMeshVec = std::vector<std::unique_ptr<ModelMesh>>;
    using DirectionalLight = std::unique_ptr<Light::DirectionalLight>;
    using PointLightVec = std::vector<std::unique_ptr<Light::PointLight>>;

    class Scene
    {
    private:
        ModelMeshVec        m_pModelMeshVec;
        DirectionalLight    m_pDirectionalLight;
        PointLightVec       m_pPointLightVec;

        Graphics::IDX12Resouce* pConstantBufferResource;

    private:
        Scene(const Scene&) = delete;
        Scene operator=(const Scene&) = delete;

    public:
        Scene();
        ~Scene();

        void SetUpScene(Graphics::IGraphics* graphics, const UINT windowWidth, const UINT windowHeight);

        void UpdateScene(Graphics::IGraphics* graphics);

        void DrawScene(Graphics::IGraphics* graphics);
    };
} // namespace Simple

#endif // _SCENE_H_
