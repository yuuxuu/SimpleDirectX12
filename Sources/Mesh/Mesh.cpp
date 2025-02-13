/**
 * @file Mesh.cpp
 * @brief
 * @author Yu Kimura
 * @date 2025/02/02
 */

#include "Mesh.h"

#include "Utility/utility.h"

#include "Param/BufferParam.h"

#include "Buffer/VertexBuffer.h"

#include "Material/Material.h"

#include "Texture/Texture.h"

namespace Simple {

    // コンストラクタ
    Mesh::Mesh() :
        m_vertices(),
        m_indecies(),
        pConstantBufferResource(),
        pVertexBufferResource(),
        pIndexBufferResource()
    {}

    // デストラクタ
    Mesh::~Mesh()
    {
        VecClear(m_vertices);
        VecClear(m_indecies);
    }

    void Mesh::Initialize(const std::vector<VertexBuffer>& vertices, const std::vector<DWORD>& indecies)
    {
        m_vertices = vertices;
        m_indecies = indecies;
    }

    void Mesh::InitializeGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        BufferParam vertexBufferParam;
        vertexBufferParam.byteWidth = sizeof(VertexBuffer) * (UINT)m_vertices.size();
        vertexBufferParam.byteWidthStride = sizeof(VertexBuffer);

        pGraphics->InitializeGraphicsBufferResource(pVertexBufferResource, &vertexBufferParam, Graphics::GraphicsResourceType::VBV);

        pGraphics->UpdateGraphicsBufferResource(pVertexBufferResource, &m_vertices[0], &vertexBufferParam, Graphics::GraphicsResourceType::VBV);

        if (m_indecies.empty())
            return;

        BufferParam indexBufferParam;
        indexBufferParam.byteWidth = sizeof(DWORD) * (UINT)m_indecies.size();
        indexBufferParam.byteWidthStride = sizeof(DWORD);

        pGraphics->InitializeGraphicsBufferResource(pIndexBufferResource, &indexBufferParam, Graphics::GraphicsResourceType::IBV);

        pGraphics->UpdateGraphicsBufferResource(pIndexBufferResource, &m_indecies[0], &indexBufferParam, Graphics::GraphicsResourceType::IBV);
    }

    void Mesh::SetGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        if(m_indecies.empty())
            pGraphics->DrawInstancedVertexBuffer(pVertexBufferResource, (UINT)m_vertices.size());
        else
            pGraphics->DrawIndexedIndexBuffer(pVertexBufferResource, pIndexBufferResource, (UINT)m_indecies.size());
    }

} // namespace