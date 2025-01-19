/**
 * @file Scene.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/19
 */

#include "Scene.h"

#include "Camera/Camera.h"

namespace Simple 
{
    Scene::Scene(Graphics::IGraphics* graphics) :
        pGraphics(graphics)
    {}

    Scene::~Scene()
    {}

    void Scene::SetUpScene(const UINT windowWidth, const UINT windowHeight)
    {
        m_pCamera = std::make_unique<Camera>(windowWidth, windowHeight);
        m_pCamera->InitializeGraphicsResource(pGraphics);
    }

    void Scene::UpdateScene(const UINT windowWidth, const UINT windowHeight)
    {
        pGraphics->UpdateGraphics(windowWidth, windowHeight);
    }

} // Simple