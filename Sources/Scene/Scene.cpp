/**
 * @file Scene.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/19
 */

#include "Scene.h"

#include "Param/BufferParam.h"

#include "Camera/Camera.h"

#include "Mesh/Mesh.h"
#include "Mesh/ModelMesh.h"

#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"

#include "System/ThreadPoolSystem.h"

namespace Simple 
{
    Scene::Scene() : 
        pConstantBufferResource()
    {}

    Scene::~Scene()
    {}

    void Scene::SetUpScene(Graphics::IGraphics* pGraphics, const UINT windowWidth, const UINT windowHeight)
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<float> rand(-100.0f, 100.0f);
        std::uniform_real_distribution<float> randColor(0.0f, 1.0f);

        auto itr = std::filesystem::recursive_directory_iterator("Resources/Model/");
        for (auto path : itr)
        {
            auto extension = itr->path().extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
            if (extension.empty())
                continue;

            System::ThreadPoolSystem::GetThreadPoolSystem().AddTask(
                [=]() 
                {
                    auto pModelMesh = std::make_unique<ModelMesh>();

                    pModelMesh->InitializeGraphicsResource(pGraphics);

                    m_pModelMeshVec.push_back(std::move(pModelMesh));
                });
        }

        m_pDirectionalLight = std::make_unique<Light::DirectionalLight>();
        m_pDirectionalLight->InitializeGraphicsResource(pGraphics);

        auto numLight = 100;
        PointLightConstantBuffer pointLightConstantBuffer;
        for (auto i = 0; i < numLight; ++i)
        {
            PointLightBuffer pointLightBuffer;

            pointLightBuffer.pos = VECTOR3(rand(mt), rand(mt), rand(mt));
            pointLightBuffer.diffuse = VECTOR4(randColor(mt), randColor(mt), randColor(mt), 1.0f);
            pointLightBuffer.attenuation = VECTOR4(0.1f, 0.1f, 0.1f, 1.0f);
            pointLightBuffer.radius = 30.0f;

            pointLightConstantBuffer.pointLightBuffers[i] = pointLightBuffer;

            auto pPointLight = std::make_unique<Light::PointLight>(pointLightBuffer);
            m_pPointLightVec.push_back(std::move(pPointLight));
        }

        pointLightConstantBuffer.numPointLight = numLight;

        BufferParam param;
        param.byteWidth = sizeof(PointLightConstantBuffer);
        param.byteWidthStride = sizeof(PointLightConstantBuffer);

        pGraphics->InitializeGraphicsBufferResource(pConstantBufferResource, &param, Graphics::GraphicsResourceType::CBV);

        pGraphics->UpdateGraphicsBufferResource(pConstantBufferResource,&pointLightConstantBuffer, &param, Graphics::GraphicsResourceType::CBV);
    }

    void Scene::UpdateScene(Graphics::IGraphics* pGraphics)
    {
        
    }

    void Scene::DrawScene(Graphics::IGraphics* pGraphics)
    {
        m_pDirectionalLight->SetGraphicsResource(pGraphics);

        pGraphics->SetConstantBufferResource(4, pConstantBufferResource);

        for (auto itr = m_pModelMeshVec.cbegin(); itr != m_pModelMeshVec.cend(); itr++)
            itr->get()->SetGraphicsResource(pGraphics);
    }

} // Simple