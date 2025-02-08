/**
 * @file Mesh.h
 * @brief
 * @author Yu Kimura
 * @date 2025/02/02
 */

#pragma once

#ifndef _CMESH_H_
#define _CMESH_H_

#include "Math/math.h"

#include "GraphicsAPI/IGraphics.h"
#include "GraphicsAPI/IGraphicsResource.h"

namespace Simple 
{
    class Material;

    struct VertexBuffer;

    struct MeshInfo 
    {
        VECTOR3 vertexE;
        VECTOR3 vertexCenterPos;
        VECTOR3 maxVertexPos;
        VECTOR3 minVertexPos;

        MeshInfo() :
            vertexE(0.0f, 0.0f, 0.0f),
            vertexCenterPos(0.0f, 0.0f, 0.0f),
            maxVertexPos(0.0f, 0.0f, 0.0f),
            minVertexPos(10000.0f, 10000.0f, 10000.0f)
        {}
    };

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

        void Initialize(Graphics::IGraphics* pGraphics);

    public:
        Mesh();
        ~Mesh();

        void Initialize(const std::vector<VertexBuffer>& vertices, const std::vector<DWORD>& indecies);

        void InitializeGraphicsResource(Graphics::IGraphics* pGraphics);

        void SetGraphicsResource(Graphics::IGraphics* pGraphics);
    };

} // namespace

#endif // _CMESH_H_
