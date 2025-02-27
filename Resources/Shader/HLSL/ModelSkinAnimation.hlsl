//*****************************************************************************
//!	@file	ModelSkinAnimation.hlsl
//!	@brief	
//!	@note	
//!	@author	2020/09/16 作成：Yu Kimura
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"
#include "../ShaderUlity.hlsli"

#define BONE_INDEX_MAX 4

struct VS_IN 
{
    float3 pos        : POSITION;
    float4 color      : COLOR;
    float2 uv         : TEXCOORD;
    float3 normal     : NORMAL;
    float3 tangent    : TANGENT;
    float3 binormal   : BINORMAL;
    int4   boneIndex  : BONEINDEX;
    float4 boneWeight : BONEWEIGHT;
};

struct VS_OUT 
{
    float4 pos      : SV_POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD;
    float3 normal   : NORMAL;
    float4 posw	    : POSITION0;
};

struct PS_OUT 
{
    float4 color    : SV_TARGET;
};

[RootSignature(RS_MODEL)]
VS_OUT VS_main(VS_IN input) 
{
    VS_OUT output = (VS_OUT)0;
    
    // 頂点ブレンド処理
    float4x4 comb = GetIdentityMatrix();
    
    for (int i = 0; i < BONE_INDEX_MAX; ++i)
    {
        if (input.boneIndex[i] < 0)
            continue;
            
        comb += BoneMatrixW[input.boneIndex[i]] * input.boneWeight[i];
    }
    
    output.pos = mul(float4(input.pos, 1.0f), comb);
    output.posw = mul(output.pos, matW);
    output.pos = mul(output.posw, matVP);

    output.color = input.color;
    output.uv = input.uv;

    output.normal = mul(input.normal, (float3x3) matW);

    return output;
}

typedef VS_OUT PS_IN;

PS_OUT PS_main(PS_IN input) 
{
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