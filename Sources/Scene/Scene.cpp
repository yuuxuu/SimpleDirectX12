/**
 * @file Scene.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/19
 */

#include "Scene.h"

#include "Camera/Camera.h"

#include "Mesh/Mesh.h"
#include "Mesh/ModelMesh.h"

namespace Simple 
{
    Scene::Scene()
    {}

    Scene::~Scene()
    {}

    void Scene::SetUpScene(Graphics::IGraphics* pGraphics, const UINT windowWidth, const UINT windowHeight)
    {
        auto itr = std::filesystem::recursive_directory_iterator("Resources/Model/FBX");
        for (auto path : itr)
        {
            auto extension = itr->path().extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
            if (extension != ".fbx")
                continue;

            auto pModelMesh = std::make_unique<ModelMesh>();
            pModelMesh->LoadModel(itr->path().string());
            pModelMesh->InitializeGraphicsResource(pGraphics);

            m_pModelMeshVec.push_back(std::move(pModelMesh));
        }
    }

    void Scene::UpdateScene(Graphics::IGraphics* pGraphics)
    {
        for (auto itr = m_pModelMeshVec.cbegin(); itr != m_pModelMeshVec.cend(); itr++)
            itr->get()->SetGraphicsResource(pGraphics);
    }

} // Simple