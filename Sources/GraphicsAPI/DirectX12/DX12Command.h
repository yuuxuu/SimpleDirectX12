/**
 * @file DX12Command.h
 * @brief
 * @author Yu Kimura
 * @date 2021/08/18
 */

#pragma once

#ifndef _CDX12COMMAND_H_
#define _CDX12COMMAND_H_

using Microsoft::WRL::ComPtr;

namespace Graphics {

    // 前方宣言
    class DX12Device;

    class DX12Command {
    private:
        ComPtr<ID3D12CommandQueue>          m_pCommandQueue;

        ComPtr<ID3D12GraphicsCommandList>   m_pCommandList;

        ComPtr<ID3D12CommandAllocator>      m_pCommandAllocator;

        ComPtr<ID3D12Fence>                 m_pFence;

        HANDLE                              m_fence_event;

        UINT                                m_frames;

    private:
        DX12Command(const DX12Command&) = delete;
        DX12Command& operator=(const DX12Command&) = delete;

        bool CreateFence(ComPtr<ID3D12Device> pDevice);

        bool CreateCommandList(ComPtr<ID3D12Device> pCommandList, ID3D12CommandAllocator* pAllocator, D3D12_COMMAND_LIST_TYPE type);

        bool CreateCommandAllcator(ComPtr<ID3D12Device> pDevice, D3D12_COMMAND_LIST_TYPE type);

    public:
        explicit DX12Command(ComPtr<ID3D12CommandQueue>& pCommandQueue);
        ~DX12Command();

        bool Initialize(DX12Device* pDevice);

        bool CloseCommandList();

        bool ResetCommandList();

        void ExecuteComandLists();

        void WaitForPreviousFrame(ID3D12CommandQueue* pCommandQueue);

        void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE handle);

        void ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE handle, D3D12_CLEAR_FLAGS clearFlags);

        void SetBuffer(D3D12_VERTEX_BUFFER_VIEW& vbView, D3D12_INDEX_BUFFER_VIEW& ibView);

        void DrawInstanced(D3D12_PRIMITIVE_TOPOLOGY topology, UINT numVertices);

        void DrawIndexed(D3D12_PRIMITIVE_TOPOLOGY topology, UINT numIndecies);

        void SetRect(UINT width, UINT height);

        void SetViewPort(UINT x, UINT y, UINT width, UINT height);

        void SetRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE* dsHandle);

        void SetRenderTargetViews(std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE* dsHandle);

        void SetResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

        void SetDescriptorHeaps(ID3D12DescriptorHeap* pDescriptorHeap);

        void SetGraphicsRootSignature(ID3D12RootSignature* pRootSignature);
        void SetComputeRootSignature(ID3D12RootSignature* pRootSignature);

        void SetPipelineState(ID3D12PipelineState* pPipelineState);

        void SetGraphicsRootDescriptorTable(UINT index, D3D12_GPU_DESCRIPTOR_HANDLE handle);
        void SetComputeRootDescriptorTable(UINT index, D3D12_GPU_DESCRIPTOR_HANDLE handle);

        void SetGraphicsRootConstantBufferView(UINT index, D3D12_GPU_VIRTUAL_ADDRESS address);

        void SetGraphicsRootShaderResourceView(UINT index, D3D12_GPU_VIRTUAL_ADDRESS address);

        void SetGraphicsRootUnorderdAccessView(UINT index, D3D12_GPU_VIRTUAL_ADDRESS address);

        void Dispatch(UINT dispatchX, UINT dispatchY, UINT dispatchZ);

        ID3D12GraphicsCommandList* GetCommandList() const { return m_pCommandList.Get(); }
        ID3D12CommandQueue* GetCommandQueue() const { return m_pCommandQueue.Get(); }
    };

} // namespace

#endif // _CDX12COMMAND_H_



