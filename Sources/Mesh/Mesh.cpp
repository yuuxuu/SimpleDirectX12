/**
 * @file Mesh.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/02/02
 */

#include "Mesh.h"

#include "Utility/utility.h"

#include "Param/BufferParam.h"

#include "Buffer/WorldBuffer.h"

#include "Texture/Texture.h"

namespace Simple {

    // コンストラクタ
    Mesh::Mesh(Graphics::IGraphics* pGraphics) :
        pGraphics(pGraphics),
        pGraphicsResource(),
        pVertexBufferResource(),
        pIndexBufferResource(),
        m_vertexVec(),
        m_indexVec()
    {}

    // デストラクタ
    Mesh::~Mesh()
    {
        VecClear(m_vertexVec);
        VecClear(m_indexVec);
    }

    void Mesh::Initialize()
    {
        const float r = 0.5f;

        m_vertexVec.reserve(4);

        m_vertexVec = 
        {
            // vertex               // color                         // uv                // normal
            { VECTOR3(-r, r, 0.0f), VECTOR4(1.0f, 0.0f, 0.0f, 1.0f), VECTOR2(0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
            { VECTOR3( r, r, 0.0f), VECTOR4(0.0f, 1.0f, 0.0f, 1.0f), VECTOR2(1.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
            { VECTOR3(-r,-r, 0.0f), VECTOR4(0.0f, 0.0f, 1.0f, 1.0f), VECTOR2(0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
            { VECTOR3( r,-r, 0.0f), VECTOR4(1.0f, 1.0f, 1.0f, 1.0f), VECTOR2(1.0f, 1.0f), VECTOR3(0.0f, 0.0f, 1.0f), },
        };

        m_indexVec.reserve(6);

        m_indexVec = 
        {
            0, 1, 2,
            1, 2, 3,
        };

        m_pTexture = std::make_unique<Texture>(pGraphics);
    }

    void Mesh::InitializeGraphicsResource()
    {
        BufferParam param;
        param.byteWidth = sizeof(WorldBuffer);

        pGraphics->InitializeGraphicsBufferResource(pGraphicsResource, &param, Graphics::GraphicsResourceType::CBV);

        Matrix matScale;
        matScale.dx_m = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);

        Matrix matRotate;
        matRotate.dx_m = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);

        Matrix matTrans;
        matTrans.dx_m = DirectX::XMMatrixTranslation(1.0f, 1.0f, 1.0f);

        Matrix matWorld;
        matWorld.dx_m = matScale.dx_m * matRotate.dx_m * matTrans.dx_m;

        Simple::WorldBuffer worldBuffer;
        worldBuffer.matW = matWorld;

        pGraphics->UpdateGraphicsBufferResource(pGraphicsResource, &worldBuffer, Graphics::GraphicsResourceType::CBV);

        BufferParam vertexBufferParam;
        vertexBufferParam.byteWidth = sizeof(Vertex) * (UINT)m_vertexVec.size();
        vertexBufferParam.byteWidthStride = sizeof(Vertex);

        pGraphics->InitializeGraphicsBufferResource(pVertexBufferResource, &vertexBufferParam, Graphics::GraphicsResourceType::VBV);

        pGraphics->UpdateGraphicsBufferResource(pVertexBufferResource, &m_vertexVec[0], Graphics::GraphicsResourceType::VBV);

        if (m_indexVec.empty())
            return;

        BufferParam indexBufferParam;
        indexBufferParam.byteWidth = sizeof(DWORD) * (UINT)m_indexVec.size();
        indexBufferParam.byteWidthStride = sizeof(DWORD);

        pGraphics->InitializeGraphicsBufferResource(pIndexBufferResource, &indexBufferParam, Graphics::GraphicsResourceType::IBV);

        pGraphics->UpdateGraphicsBufferResource(pIndexBufferResource, &m_indexVec[0], Graphics::GraphicsResourceType::IBV);

        auto itr = std::filesystem::recursive_directory_iterator("Resources/Texture");
        if (m_pTexture->LoadTexture(itr->path().string()))
            m_pTexture->InitializeGraphicsResource();
    }

    void Mesh::SetGraphicsResource()
    {
        m_pTexture->SetGraphicsResource();

        pGraphics->SetConstantBufferResource(0, pGraphicsResource);

        if(m_indexVec.empty())
            pGraphics->DrawInstancedVertexBuffer(pVertexBufferResource, (UINT)m_vertexVec.size());
        else
            pGraphics->DrawIndexedIndexBuffer(pVertexBufferResource, pIndexBufferResource, (UINT)m_indexVec.size());
    }

} // namespace