//*****************************************************************************
//!	@file	ModelLine.hlsl
//!	@brief	
//!	@note	
//!	@author	2020/09/16 作成：Yu Kimura
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"
#include "../ShaderUlity.hlsli"

struct VS_IN 
{
    float3 pos        : POSITION;
    float4 color      : COLOR;
    float2 uv         : TEXCOORD;
    float3 normal     : NORMAL;
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
    output.posw = mul(float4(input.pos, 1.0f), matW);
    output.pos = mul(output.posw, matVP);
    output.color = input.color;
    return output;
}

typedef VS_OUT PS_IN;

PS_OUT PS_main(PS_IN input) 
{
    PS_OUT output = (PS_OUT)0;
    output.color = input.color;
    return output;
}