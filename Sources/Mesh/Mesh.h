/**
 * @file Mesh.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/02
 */

#pragma once

#ifndef _MESH_H_
#define _MESH_H_

#include "Math/math.h"

#include "GraphicsAPI/IGraphics.h"
#include "GraphicsAPI/IGraphicsResource.h"

namespace Simple 
{
    class Material;

    struct VertexBuffer;

    class Mesh 
    {
    private:
        std::vector<VertexBuffer>       m_vertices;
        std::vector<DWORD>              m_indecies;

        Graphics::IGraphicsResource*    pConstantBufferResource;
        Graphics::IGraphicsResource*    pVertexBufferResource;
        Graphics::IGraphicsResource*    pIndexBufferResource;

    private:
        Mesh(const Mesh&) = delete;
        Mesh operator=(const Mesh&) = delete;

    public:
        Mesh();
        ~Mesh();

        void Initialize(const std::vector<VertexBuffer>& vertices, const std::vector<DWORD>& indecies);

        void InitializeGraphicsResource(Graphics::IGraphics* pGraphics);

        void SetGraphicsResource(Graphics::IGraphics* pGraphics);
    };

} // namespace

#endif // _MESH_H_
