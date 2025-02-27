/**
 * @file GraphicsPipeline.cpp
 * @brief
 * @author Yu Kimura
 * @date 2020/05/18
 */

#include "GraphicsPipeline.h"

#include "GraphicsAPI/DirectX12/DX12Device.h"
#include "GraphicsAPI/DirectX12/DX12Command.h"

#include "GraphicsAPI/Shader/DirectX/Shader.h"

namespace Graphics 
{
namespace Shader
{
    // コンストラクタ
    GraphicsPipeline::GraphicsPipeline(DX12Device* pDX12Device,DX12Command* pDX12Command) :
        pDX12Device(pDX12Device),
        pDX12Command(pDX12Command)
    {}

    // デストラクタ
    GraphicsPipeline::~GraphicsPipeline()
    {}

    void GraphicsPipeline::SetDepthStencilState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc) {
        graphicsPipelineStateDesc.DepthStencilState.DepthEnable = TRUE;
        graphicsPipelineStateDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
        graphicsPipelineStateDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        graphicsPipelineStateDesc.DepthStencilState.StencilEnable = FALSE;
        graphicsPipelineStateDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
        graphicsPipelineStateDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
        graphicsPipelineStateDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        graphicsPipelineStateDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        graphicsPipelineStateDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        graphicsPipelineStateDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
        graphicsPipelineStateDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        graphicsPipelineStateDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        graphicsPipelineStateDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        graphicsPipelineStateDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    }

    void GraphicsPipeline::SetRasterizerState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc) {
        graphicsPipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        graphicsPipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
        graphicsPipelineStateDesc.RasterizerState.FrontCounterClockwise = FALSE;
        graphicsPipelineStateDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        graphicsPipelineStateDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        graphicsPipelineStateDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        graphicsPipelineStateDesc.RasterizerState.DepthClipEnable = TRUE;
        graphicsPipelineStateDesc.RasterizerState.MultisampleEnable = FALSE;
        graphicsPipelineStateDesc.RasterizerState.AntialiasedLineEnable = FALSE;
        graphicsPipelineStateDesc.RasterizerState.ForcedSampleCount = 0;
        graphicsPipelineStateDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
    }

    void GraphicsPipeline::SetBlendState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc) {
        graphicsPipelineStateDesc.BlendState.AlphaToCoverageEnable = FALSE;
        graphicsPipelineStateDesc.BlendState.IndependentBlendEnable = FALSE;

        for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
            graphicsPipelineStateDesc.BlendState.RenderTarget[i].BlendEnable = TRUE;
            graphicsPipelineStateDesc.BlendState.RenderTarget[i].SrcBlend = D3D12_BLEND_ONE;
            graphicsPipelineStateDesc.BlendState.RenderTarget[i].DestBlend = D3D12_BLEND_ZERO;
            graphicsPipelineStateDesc.BlendState.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
            graphicsPipelineStateDesc.BlendState.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
            graphicsPipelineStateDesc.BlendState.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
            graphicsPipelineStateDesc.BlendState.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
            graphicsPipelineStateDesc.BlendState.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
            graphicsPipelineStateDesc.BlendState.RenderTarget[i].LogicOpEnable = FALSE;
            graphicsPipelineStateDesc.BlendState.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_CLEAR;
        }
    }

    bool GraphicsPipeline::InitializePipeline(D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphicsPipelineStateDesc)
    {
        graphicsPipelineStateDesc.NumRenderTargets = 1;
        graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;
        graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

        SetDepthStencilState(graphicsPipelineStateDesc);
        SetRasterizerState(graphicsPipelineStateDesc);
        SetBlendState(graphicsPipelineStateDesc);

        D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = 
        {
            { "POSITION"  , 0, DXGI_FORMAT_R32G32B32_FLOAT	 , 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR"     , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD"  , 0, DXGI_FORMAT_R32G32_FLOAT		 , 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL"    , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TANGENT"   , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "BINORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "BONEINDEX" , 0, DXGI_FORMAT_R32G32B32A32_SINT , 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };

        graphicsPipelineStateDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };
        graphicsPipelineStateDesc.SampleDesc = { 1, 0 };
        graphicsPipelineStateDesc.SampleMask = UINT_MAX;

        if (!pDX12Device->CreateGraphicsPipelineState(m_pPipelineState, graphicsPipelineStateDesc))
        {
            MessageBoxA(NULL, "グラフィックスパイプラインの作成に失敗しました。", "MessageBox", MB_OK);
            return false;
        }

        return true;
    }

    void GraphicsPipeline::SetPipelineState()
    {
        pDX12Command->SetPipelineState(m_pPipelineState.Get());
    }

} // namespace GraphicsPipeline
} // namespace Graphics