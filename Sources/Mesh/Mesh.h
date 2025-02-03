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
    class Texture;

    struct Vertex 
    {
        VECTOR3	vertex;
        VECTOR4	color;
        VECTOR2	uv;
        VECTOR3	normal;
        VECTOR3	tangent;
        VECTOR3	binormal;
    };

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
        std::unique_ptr<Texture>        m_pTexture;

        std::vector<Vertex>             m_vertexVec;
        std::vector<DWORD>              m_indexVec;

        Graphics::IGraphics*            pGraphics;
        Graphics::IGraphicsResource*    pGraphicsResource;

        Graphics::IGraphicsResource*    pVertexBufferResource;
        Graphics::IGraphicsResource*    pIndexBufferResource;


    private:
        Mesh(const Mesh&) = delete;
        Mesh operator=(const Mesh&) = delete;

    public:
        explicit Mesh(Graphics::IGraphics* pGraphics);
        ~Mesh();

        void Initialize();

        void InitializeGraphicsResource();

        void SetGraphicsResource();
    };

} // namespace

#endif // _CMESH_H_
