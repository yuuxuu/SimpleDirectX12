/**
 * @file Texture.cpp
 * @brief
 * @author Yu Kimura
 * @date 2021/05/07
 */

#include "Texture.h"

#include "Utility/utility.h"

#include "Math/math.h"

namespace Simple {

    // コンストラクタ
    Texture::Texture() :
        pGraphicsResource(),
        m_pTextureParam(),
        m_cubeTextureData(),
        m_filePath(std::string()),
        m_isLoaded(false)
    {}

    // コンストラクタ
    Texture::Texture(const std::string& filePath) :
        pGraphicsResource(),
        m_pTextureParam(),
        m_cubeTextureData(),
        m_filePath(filePath),
        m_isLoaded(false)
    {}

    // デストラクタ
    Texture::~Texture()
    {}

    // テクスチャを読み込む
    bool Texture::LoadTexture()
    {
        if (m_filePath.empty())
            return false;

        return LoadTexture(m_filePath);
    }

    bool Texture::LoadTexture(const std::string& filePath) 
    {
        if (m_isLoaded) return true;

        std::wstring loadName;
        Simple::StringConvertToWchar(filePath, loadName);

        DirectX::ScratchImage sImage;
        HRESULT hr = DirectX::LoadFromWICFile(loadName.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, sImage);
        if (FAILED(hr)) 
        {
            std::string errorStr = filePath + "読み込み：失敗";

            MessageBox(NULL, errorStr.c_str(), "MessageBox", MB_OK);
            return false;
        }

        const DirectX::Image* image = sImage.GetImages();

        m_pTextureParam = std::make_unique<TextureDataParam>();

        m_pTextureParam->PixelsSize = (UINT)sImage.GetPixelsSize();
        m_pTextureParam->pData = std::make_unique<uint8_t[]>(m_pTextureParam->PixelsSize);

        std::memcpy(m_pTextureParam->pData.get(), image->pixels, m_pTextureParam->PixelsSize);

        m_pTextureParam->RowPitch = (UINT)image->rowPitch;
        m_pTextureParam->SlicePitch = (UINT)image->slicePitch;
        m_pTextureParam->Width = (UINT)image->width;
        m_pTextureParam->Height = (UINT)image->height;
        m_pTextureParam->Format = image->format;

        m_isLoaded = true;

        return m_isLoaded;
    }

    void Texture::InitializeGraphicsResource(Graphics::IGraphics* pGraphics)
    {
        if (!m_isLoaded)
            return;

        pGraphics->InitializeGraphicsBufferResource(pGraphicsResource, m_pTextureParam.get(), Graphics::GraphicsResourceType::SRV);
    }

    void Texture::SetGraphicsResource(const UINT index, Graphics::IGraphics* pGraphics)
    {
        if (!m_isLoaded)
            return;

        pGraphics->SetShaderResource(6 + index, pGraphicsResource);
    }

    // テクスチャをキューブマップテクスチャに変換
    void Texture::TextureConvertToCubeMapTexture() {

        if (!m_pTextureParam) return;

        const UINT cubeSize = 512;
        const UINT pixelSize = 4;

        m_cubeTextureData->Width = cubeSize;
        m_cubeTextureData->Height = cubeSize;
        m_cubeTextureData->RowPitch = cubeSize * pixelSize;
        m_cubeTextureData->SlicePitch = m_cubeTextureData->RowPitch * cubeSize;
        m_cubeTextureData->PixelsSize = m_cubeTextureData->SlicePitch;
        m_cubeTextureData->Format = m_pTextureParam->Format;

        for (int i = 0; i < CUBE_FACE_MAX; ++i) {
            m_cubeTextureData->pDatas[i] = std::make_unique<uint8_t[]>(m_cubeTextureData->PixelsSize);

            for (UINT h = 0; h < m_cubeTextureData->Height; ++h) {
                for (UINT w = 0; w < m_cubeTextureData->Width; ++w) {
                    VECTOR2 uv = { (float)w / m_cubeTextureData->Width, (float)h / m_cubeTextureData->Height };

                    VECTOR3 faceVector;
                    switch (i) {
                    case 0:
                        faceVector = { 
                            1.0f,
                            2.0f * -uv.y + 1.0f,
                            2.0f * -uv.x + 1.0f
                        };
                        break;
                    case 1:
                        faceVector = { 
                            -1.0f,
                            2.0f * -uv.y + 1.0f,
                            2.0f *  uv.x - 1.0f
                        };
                        break;
                    case 2:
                        faceVector = {
                            2.0f * uv.x - 1.0f,
                            1.0f,
                            2.0f *  uv.y - 1.0f
                        };
                        break;
                    case 3:
                        faceVector = {
                            2.0f * uv.x - 1.0f,
                            -1.0f,
                            2.0f * -uv.y + 1.0f
                        };
                        break;
                    case 4:
                        faceVector = {
                            2.0f * uv.x - 1.0f,
                            2.0f * -uv.y + 1.0f,
                            1.0f
                        };
                        break;
                    case 5:
                        faceVector = {
                            2.0f * -uv.x + 1.0f,
                            2.0f * -uv.y + 1.0f,
                            -1.0f
                        };
                        break;
                    }

                    float len = Vec3Lenght(faceVector);

                    float theta = atan2f(faceVector.z, faceVector.x);
                    float phi = std::acosf(faceVector.y / len);

                    uv.x = theta / (2.0f * PI);
                    if (uv.x < 0) uv.x += 1.0f;
                    uv.y = phi / PI;

                    UINT imageXPixel = UINT(m_pTextureParam->Width * uv.x);
                    UINT imageYPixel = UINT(m_pTextureParam->Height * uv.y);

                    uint8_t r = m_pTextureParam->pData[imageXPixel * pixelSize + imageYPixel * m_pTextureParam->RowPitch];
                    uint8_t g = m_pTextureParam->pData[imageXPixel * pixelSize + imageYPixel * m_pTextureParam->RowPitch + 1];
                    uint8_t b = m_pTextureParam->pData[imageXPixel * pixelSize + imageYPixel * m_pTextureParam->RowPitch + 2];

                    m_cubeTextureData->pDatas[i][w * pixelSize + h * m_cubeTextureData->RowPitch] = r;
                    m_cubeTextureData->pDatas[i][w * pixelSize + h * m_cubeTextureData->RowPitch + 1] = g;
                    m_cubeTextureData->pDatas[i][w * pixelSize + h * m_cubeTextureData->RowPitch + 2] = b;
                    m_cubeTextureData->pDatas[i][w * pixelSize + h * m_cubeTextureData->RowPitch + 3] = 0xff;
                }
            }
        }
    }

} // namespace