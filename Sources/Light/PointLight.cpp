/**
 * @file PointLight.cpp
 * @brief
 * @author Yu Kimura
 * @date 2021/08/02
 */

#include "PointLight.h"

#include "math/math.h"

#include "Mesh/Mesh.h"
#include "Mesh/ModelMesh.h"

#include "Buffer/VertexBuffer.h"
#include "Buffer/WorldBuffer.h"

#include "Param/ModelDrawInfoParam.h"


namespace Simple 
{
namespace Light
{
    // コンストラクタ
    PointLight::PointLight(const Simple::PointLightBuffer& pointLightBuffer) :
        m_pointLightBuffer(pointLightBuffer)
    {}

    // デストラクタ
    PointLight::~PointLight()
    {}

    void PointLight::InitializeGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        const float r = 0.5f;

        std::vector<Simple::VertexBuffer> vertices =
        {
            // vertex               // color                         // uv                // normal
            { VECTOR3(-r, r, 0.0f), VECTOR4(1.0f, 0.0f, 0.0f, 1.0f), VECTOR2(0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
            { VECTOR3(r, r, 0.0f), VECTOR4(0.0f, 1.0f, 0.0f, 1.0f), VECTOR2(1.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
            { VECTOR3(-r,-r, 0.0f), VECTOR4(0.0f, 0.0f, 1.0f, 1.0f), VECTOR2(0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
            { VECTOR3(r,-r, 0.0f), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 1.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
        };

        std::vector<DWORD> indecies =
        {
            0, 1, 2,
            1, 2, 3,
        };

        pModelMesh = std::make_unique<ModelMesh>();

        auto pMesh = std::make_unique<Mesh>();

        ModelDrawInfoParam param;
        param.pMesh = pMesh.get();

        pMesh->Initialize(vertices, indecies);

        pModelMesh->RegisterMesh(pMesh);
        pModelMesh->AddModelDrawInfoParam(param);
        pModelMesh->InitializeGraphicsResource(pGraphics);
    }

    void PointLight::SetGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        Matrix matScale;
        auto scale = 30.0f;
        matScale.dx_m = DirectX::XMMatrixScaling(scale, scale, scale);

        Matrix matRotate;
        matRotate.dx_m = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);

        Matrix matTrans;
        auto pos = m_pointLightBuffer.pos;
        matTrans.dx_m = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

        Matrix matWorld;
        matWorld.dx_m = matScale.dx_m * matRotate.dx_m * matTrans.dx_m;

        Simple::WorldBuffer worldBuffer;
        worldBuffer.matW.dx_m = DirectX::XMMatrixTranspose(matWorld.dx_m);

        pModelMesh->UpdateGraphicsResource(pGraphics, worldBuffer);
        pModelMesh->SetGraphicsResource(pGraphics);
    }

    void PointLight::SetPointLightBuffer(const Simple::PointLightBuffer& pointLightBuffer)
    {
        m_pointLightBuffer = pointLightBuffer;
    }

} // namespace Light
} // namespace Simple
