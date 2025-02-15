/**
 * @file Scene.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/01/19
 */

#include "Scene.h"

#include "Param/BufferParam.h"
#include "Param/ModelDrawInfoParam.h"

#include "Buffer/VertexBuffer.h"
#include "Material/Material.h"

#include "Camera/Camera.h"

#include "Mesh/Mesh.h"
#include "Mesh/ModelMesh.h"

#include "Material/Material.h"

#include "Texture/Texture.h"

#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"

#include "ModelLoader/FBX/FBXLoader.h"
#include "ModelLoader/OBJ/OBJLoader.h"
#include "ModelLoader/PMX/PMXLoader.h"

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
        auto itr = std::filesystem::recursive_directory_iterator("Resources/Model/");
        for (auto path : itr)
        {
            auto extension = path.path().extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
            if (extension.empty())
                continue;

            std::unique_ptr<ModelLoader::IModelLoader> pModelLoader = nullptr;

            if (extension == ".fbx")
                pModelLoader = std::make_unique<ModelLoader::FBXLoader>();
            else if (extension == ".obj")
                pModelLoader = std::make_unique<ModelLoader::OBJLoader>();
            else if (extension == ".pmx")
                pModelLoader = std::make_unique<ModelLoader::PMXLoader>();

            if (!pModelLoader)
                continue;

            auto pModelMesh = std::make_unique<ModelMesh>();

            pModelLoader->LoadModel(path.path().string(), pModelMesh.get());

            pModelMesh->InitializeGraphicsResource(pGraphics);

            m_pModelMeshVec.push_back(std::move(pModelMesh));

            //System::ThreadPoolSystem::GetThreadPoolSystem().AddTask(
            //    [=]() 
            //    {
            //        
            //    });
        }

        {
            auto pModelMesh = std::make_unique<ModelMesh>();

            ModelDrawInfoParam param;

            auto pMesh = std::make_unique<Mesh>();
            param.pMesh = pMesh.get();

            const float r = 500.0f;

            std::vector<VertexBuffer> vertices =
            {
                // vertex               // color                         // uv                // normal
                { VECTOR3(-r, 0.0f, r), VECTOR4(1.0f, 0.0f, 0.0f, 1.0f), VECTOR2(0.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f), },
                { VECTOR3( r, 0.0f, r), VECTOR4(0.0f, 1.0f, 0.0f, 1.0f), VECTOR2(1.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f), },
                { VECTOR3(-r, 0.0f,-r), VECTOR4(0.0f, 0.0f, 1.0f, 1.0f), VECTOR2(0.0f, 1.0f), VECTOR3(0.0f, 1.0f, 0.0f), },
                { VECTOR3( r, 0.0f,-r), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 1.0f), VECTOR3(0.0f, 1.0f, 0.0f), },
            };

            std::vector<DWORD> indecies =
            {
                0, 1, 2,
                1, 2, 3,
            };

            pMesh->Initialize(vertices, indecies);

            pModelMesh->RegisterMesh(pMesh);

            auto pTexture = std::make_unique<Simple::Texture>();
            std::vector<Texture*> vecTexture;
            vecTexture.push_back(pTexture.get());
            pModelMesh->RegisterTexture("Resources/Texture/default.png", pTexture);

            auto pMaterial = std::make_unique<Material>();

            MaterialBuffer materialBuffer;
            materialBuffer.diffuse = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

            pMaterial->Initialize(materialBuffer);

            param.pMaterialTexturesMap[pMaterial.get()] = vecTexture;

            pModelMesh->RegisterMaterial(pMaterial);

            pModelMesh->AddModelDrawInfoParam(param);

            pModelMesh->InitializeGraphicsResource(pGraphics);

            m_pModelMeshVec.push_back(std::move(pModelMesh));
        }

        {
            m_pDirectionalLight = std::make_unique<Light::DirectionalLight>();
            m_pDirectionalLight->InitializeGraphicsResource(pGraphics);

            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<float> randPos(-500.0f, 500.0f);
            std::uniform_real_distribution<float> randColor(0.1f, 1.0f);

            PointLightConstantBuffer pointLightConstantBuffer;

            auto numLight = 100;
            for (auto i = 0; i < numLight; ++i)
            {
                PointLightBuffer pointLightBuffer;

                pointLightBuffer.pos = VECTOR3(randPos(mt), 10.0f, randPos(mt));
                pointLightBuffer.diffuse = VECTOR4(randColor(mt), randColor(mt), randColor(mt), 1.0f);
                pointLightBuffer.attenuation = VECTOR4(0.1f, 0.1f, 0.0f, 1.0f);
                pointLightBuffer.radius = 50.0f;

                pointLightConstantBuffer.pointLightBuffers[i] = pointLightBuffer;

                auto pPointLight = std::make_unique<Light::PointLight>(pointLightBuffer);
                m_pPointLightVec.push_back(std::move(pPointLight));
            }

            pointLightConstantBuffer.numPointLight = numLight;

            BufferParam param;
            param.byteWidth = sizeof(PointLightConstantBuffer);
            param.byteWidthStride = sizeof(PointLightConstantBuffer);

            pGraphics->InitializeGraphicsBufferResource(pConstantBufferResource, &param, Graphics::GraphicsResourceType::CBV);

            pGraphics->UpdateGraphicsBufferResource(pConstantBufferResource, &pointLightConstantBuffer, &param, Graphics::GraphicsResourceType::CBV);
        }
    }

    void Scene::UpdateScene(Graphics::IGraphics* pGraphics)
    {
        
    }

    void Scene::DrawScene(Graphics::IGraphics* pGraphics)
    {
        m_pDirectionalLight->SetGraphicsResource(pGraphics);

        pGraphics->SetConstantBufferResource(4, pConstantBufferResource);

        //for (auto itr = m_pPointLightVec.cbegin(); itr != m_pPointLightVec.cend(); itr++)
        //    itr->get()->SetGraphicsResource(pGraphics);

        for (auto itr = m_pModelMeshVec.cbegin(); itr != m_pModelMeshVec.cend(); itr++)
            itr->get()->SetGraphicsResource(pGraphics);
    }

} // Simple