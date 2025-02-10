/**
 * @file PointLight.cpp
 * @brief
 * @author Yu Kimura
 * @date 2021/08/02
 */

#include "PointLight.h"

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

    void PointLight::SetPointLightBuffer(const Simple::PointLightBuffer& pointLightBuffer)
    {
        m_pointLightBuffer = pointLightBuffer;
    }

} // namespace Light
} // namespace Simple
