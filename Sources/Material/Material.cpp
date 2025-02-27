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
    Material::Material(const Simple::MaterialBuffer& materialBuffer) :
        m_MaterialBuffer(materialBuffer),
        pGraphicsResource(nullptr)
    {}

    // デストラクタ
    Material::~Material()
    {}

    void Material::InitializeGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        BufferParam materialBufferParam;
        materialBufferParam.byteWidth = sizeof(MaterialBuffer);
        materialBufferParam.byteWidthStride = sizeof(MaterialBuffer);

        pGraphics->InitializeGraphicsBufferResource(pGraphicsResource, &materialBufferParam, Graphics::GraphicsResourceType::CBV);

        pGraphics->UpdateGraphicsBufferResource(pGraphicsResource, &m_MaterialBuffer, &materialBufferParam, Graphics::GraphicsResourceType::CBV);
    }

    void Material::SetGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        if (!pGraphicsResource)
            return;

        pGraphics->SetConstantBufferView(2, pGraphicsResource);
    }

} // namespace