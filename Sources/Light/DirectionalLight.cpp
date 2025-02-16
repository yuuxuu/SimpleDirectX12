/**
 * @file DirectionalLight.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/02/08
 */

#include "DirectionalLight.h"

#include "Param/BufferParam.h"
#include "Buffer/LightBuffer.h"

namespace Simple 
{
namespace Light
{
    // コンストラクタ
    DirectionalLight::DirectionalLight() :
        pConstantBufferResource()
    {}

    // デストラクタ
    DirectionalLight::~DirectionalLight()
    {}

    void DirectionalLight::InitializeGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        Simple::BufferParam param;
        param.byteWidth = sizeof(Simple::DirectionalLightBuffer);
        param.byteWidthStride = sizeof(Simple::DirectionalLightBuffer);

        pGraphics->InitializeGraphicsBufferResource(pConstantBufferResource, &param, Graphics::GraphicsResourceType::CBV);

        Simple::DirectionalLightBuffer directionalLight;
        directionalLight.pos = Simple::VECTOR4(0.0f, FLT_MAX, -FLT_MAX, 1.0f);
        directionalLight.diffuse = Simple::VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

        pGraphics->UpdateGraphicsBufferResource(pConstantBufferResource, &directionalLight, &param, Graphics::GraphicsResourceType::CBV);
    }

    void DirectionalLight::SetGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        pGraphics->SetConstantBufferResource(3, pConstantBufferResource);
    }

} // namespace Light
} // namespace Simple
