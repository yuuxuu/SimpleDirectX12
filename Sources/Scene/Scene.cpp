/**
 * @file Scene.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/19
 */

#include "Scene.h"

#include "Camera/Camera.h"

#include "Mesh/Mesh.h"

namespace Simple 
{
    Scene::Scene(Graphics::IGraphics* graphics) :
        pGraphics(graphics)
    {}

    Scene::~Scene()
    {}

    void Scene::SetUpScene(const UINT windowWidth, const UINT windowHeight)
    {
        m_pCamera = std::make_unique<Camera>(pGraphics, windowWidth, windowHeight);
        m_pCamera->InitializeGraphicsResource();

        auto pMesh = std::make_unique<Mesh>(pGraphics);
        pMesh->Initialize();
        pMesh->InitializeGraphicsResource();

        m_pMeshVec.push_back(std::move(pMesh));
    }

    void Scene::UpdateScene(const UINT windowWidth, const UINT windowHeight)
    {
        pGraphics->Update(windowWidth, windowHeight);

        m_pCamera->SetGraphicsResource();

        for (auto itr = m_pMeshVec.cbegin(); itr != m_pMeshVec.cend(); itr++)
            itr->get()->SetGraphicsResource();

        pGraphics->Finalize();
    }

} // Simple