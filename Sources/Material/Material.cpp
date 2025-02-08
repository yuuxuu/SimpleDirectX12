/**
 * @file Material.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/02/05
 */

#include "Material.h"

#include "Param/BufferParam.h"

namespace Simple {

    // コンストラクタ
    Material::Material() :
        m_MaterialBuffer(),
        pGraphicsResource(nullptr)
    {}

    // デストラクタ
    Material::~Material()
    {}

    void Material::Initialize(const MaterialBuffer& materialBuffer)
    {
        m_MaterialBuffer = materialBuffer;
    }

    void Material::InitializeGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        BufferParam materialBufferParam;
        materialBufferParam.byteWidth = sizeof(MaterialBuffer);

        pGraphics->InitializeGraphicsBufferResource(pGraphicsResource, &materialBufferParam, Graphics::GraphicsResourceType::CBV);

        pGraphics->UpdateGraphicsBufferResource(pGraphicsResource, &m_MaterialBuffer, &materialBufferParam, Graphics::GraphicsResourceType::CBV);
    }

    void Material::SetGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        pGraphics->SetConstantBufferResource(2, pGraphicsResource);
    }

} // namespace