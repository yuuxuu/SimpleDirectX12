/**
 * @file stdafx.h
 * @brief
 * @author Yu Kimura
 * @date 2021/04/09
 */

#pragma once

// windows
#include <windows.h>
#include <stdio.h>
#include <winnt.h>

// std
#include <vector>
#include <array>
#include <list>
#include <map>
#include <queue>
#include <string>
#include <regex>
#include <functional>
#include <thread>
#include <mutex>
#include <random>

// FI/FO
#include <iostream>
#include <fstream>
#include <sstream>

#include <filesystem>

 // D3D11
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

// D3D12
#include <d3d12.h>
#include <d3d12shader.h>
#pragma comment(lib, "d3d12.lib")

// d3dCompiler
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

// dxgi
#include <dxgi1_4.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

// D3DX
#include <D3DX12/d3dx12.h>

// DirectXMath
//#include <DirectXMath/DirectXMath.h>

// DirectXTex
#include <DirectXTex/DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")

// FBXSDK
//#include <fbxsdk.h>
//#pragma comment(lib, "libfbxsdk-mt.lib")

// input
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")

// rpc
#include <rpc.h>
#pragma comment(lib, "Rpcrt4.lib")


//#include <wrl/client.h>
//#pragma comment(lib, "winmm.lib")