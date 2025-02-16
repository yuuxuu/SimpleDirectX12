//*****************************************************************************
//!	@file	Model.hlsl
//!	@brief	
//!	@note	
//!	@author	2020/09/16 作成：Yu Kimura
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"
#include "../ShaderUlity.hlsli"

struct VS_IN {
    float3 pos      : POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD;
    float3 normal   : NORMAL;
};

struct VS_OUT {
    float4 pos      : SV_POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD;
    float3 normal   : NORMAL;
    float4 posw	    : POSITION0;
};

struct PS_OUT {
    float4 color    : SV_TARGET;
};

[RootSignature(RS_MODEL)]
VS_OUT VS_main(VS_IN input) {
    VS_OUT output = (VS_OUT)0;

    output.posw = mul(float4(input.pos, 1.0f), matW);
    output.pos = mul(output.posw, matVP);

    output.color = input.color;
    output.uv = input.uv;

    output.normal = mul(input.normal, (float3x3)matW);

    return output;
}

typedef VS_OUT PS_IN;

PS_OUT PS_main(PS_IN input) {
    PS_OUT output = (PS_OUT)0;

    output.color = diffuseTex.Sample(sampleState, input.uv);
    output.color *= material.diffuse;
    
    if (output.color.a <= 0.3f)
        discard;
    
    float3 dir = normalize(-directionalLight.pos.xyz);
    float3 n = normalize(input.normal);
    float d = HalfLambert(n, dir);
    float4 dLight = directionalLight.diffuse * d;

    float4 pLight;
    for (int i = 0; i < numPointLight; ++i) 
    {
        float3 dir = pointLights[i].pos - input.posw.xyz;
        float len = length(dir);
    
        dir = normalize(dir);
        // 光源と法線の内積を計算
        float d = Lambert(n, dir);
        // 減衰
        float att = Attenuation(pointLights[i].attenuation.xyz, len);
    
        pLight += (pointLights[i].diffuse * d) * att;
    }
    
    output.color *= dLight + pLight;

    return output;
}