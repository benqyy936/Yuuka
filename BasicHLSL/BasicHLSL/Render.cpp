#include "stdafx.h"
#include <d3d11_1.h>
#include <dxgi.h>
#include <D3DCompiler.h>
#include "Render.h"
#include "WICTextureLoader.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")

//Global Varibles
ID3D11Device     *g_pD3D11Device = NULL;
IDXGISwapChain   *g_pDXGISwapChain =NULL;
ID3D11DeviceContext *g_pD3D11DeviceContext =NULL;
ID3D11InputLayout   *g_pD3D11InputLayout =NULL;
ID3D11VertexShader  *g_pVertexShader =NULL;
ID3D11PixelShader   *g_pPixelShader =NULL;
ID3D11Buffer        *g_pVertexBuffer =NULL;
ID3D11Buffer        *g_pIndexBuffer =NULL;
ID3D11ShaderResourceView *g_pShaderResourceView = NULL;
ID3D11SamplerState  *g_pSamplerState = NULL;
ID3D11RenderTargetView *g_pRenderTargetView =NULL;
ID3D11DepthStencilView *g_pDepthStencilView =NULL;
ID3D11RasterizerState *g_pRasterize = NULL;
D3D_FEATURE_LEVEL g_featureLevel;

HRESULT CreateD3D11Device(HWND hWnd)
{
    D3D_FEATURE_LEVEL pD3DFeatureLevel[] = 
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    D3D_DRIVER_TYPE pDriverType[] = 
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };

    DWORD driverTypeNum = ARRAYSIZE(pDriverType);
    HRESULT hr = S_OK;
    RECT clientRect = { 0 };
    GetClientRect(hWnd, &clientRect);
    DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.Width = clientRect.right - clientRect.left;
    swapChainDesc.BufferDesc.Height = clientRect.bottom - clientRect.top;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;

    for (DWORD i = 0; i < driverTypeNum; i++)
    {
        hr = D3D11CreateDeviceAndSwapChain(NULL, pDriverType[i], NULL, D3D11_CREATE_DEVICE_DEBUG, pD3DFeatureLevel, ARRAYSIZE(pD3DFeatureLevel), D3D11_SDK_VERSION, &swapChainDesc,
            &g_pDXGISwapChain, &g_pD3D11Device, &g_featureLevel, &g_pD3D11DeviceContext);
        if(SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
        return hr;
	return hr;
}

HRESULT InitD3D11Device()
{
	HRESULT hr = S_OK; 
	ID3DBlob *pVsShaderBlob;
	ID3DBlob *pPsShaderBlob;
	DWORD dwShaderFlags = 0;
#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
    //Create Input Layout and Vertex Shader
	D3D11_INPUT_ELEMENT_DESC  pInputElementDesc[] = 
	{ 
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	hr = D3DCompileFromFile(L"VS.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", dwShaderFlags, 0, &pVsShaderBlob, NULL);
	g_pD3D11Device->CreateInputLayout(pInputElementDesc, ARRAYSIZE(pInputElementDesc), pVsShaderBlob->GetBufferPointer(), pVsShaderBlob->GetBufferSize(), &g_pD3D11InputLayout);
    g_pD3D11Device->CreateVertexShader(pVsShaderBlob->GetBufferPointer(), pVsShaderBlob->GetBufferSize(), NULL, &g_pVertexShader);
    pVsShaderBlob->Release();
    //Create Vexter Buffer
    InputVertex pInputVertex[] = { 
        { XMFLOAT3(-1.0, -1.0,0),XMFLOAT2(0.0f,1.0f) },
        { XMFLOAT3( -1.0, 1.0,0),XMFLOAT2(0.0f,0.0f) },
        { XMFLOAT3( 1.0,1.0,0),XMFLOAT2(1.0,0.0) },
        { XMFLOAT3(1.0,-1.0,0),XMFLOAT2(1.0,1.0) }
    };
    D3D11_BUFFER_DESC descVB = { 0 };
    D3D11_SUBRESOURCE_DATA  VBInitData = { 0 };
    VBInitData.pSysMem = pInputVertex;
    VBInitData.SysMemPitch = sizeof(pInputVertex);
    VBInitData.SysMemSlicePitch = sizeof(pInputVertex);
    descVB.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    descVB.Usage = D3D11_USAGE_IMMUTABLE;
    descVB.ByteWidth = sizeof(pInputVertex);
    descVB.CPUAccessFlags = 0;
    descVB.MiscFlags = 0;
    g_pD3D11Device->CreateBuffer(&descVB, &VBInitData, &g_pVertexBuffer);
    //Create Index Buffer
    D3D11_BUFFER_DESC descIB = { 0 };
    WORD pIBData[] = 
    { 
        0,1,2,
        0,2,3
    };
    D3D11_SUBRESOURCE_DATA IBInitData = { 0 };
    IBInitData.pSysMem = pIBData;
    IBInitData.SysMemPitch = sizeof(pIBData);
    descIB.BindFlags = D3D11_BIND_INDEX_BUFFER;
    descIB.ByteWidth = sizeof(pIBData);
    descIB.Usage = D3D11_USAGE_IMMUTABLE;
    g_pD3D11Device->CreateBuffer(&descIB, &IBInitData, &g_pIndexBuffer);
    //Create Pixel Shader
    D3DCompileFromFile(L"PS.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", dwShaderFlags, 0, &pPsShaderBlob, NULL);
    g_pD3D11Device->CreatePixelShader(pPsShaderBlob->GetBufferPointer(), pPsShaderBlob->GetBufferSize(), NULL, &g_pPixelShader);
    //Create Render Target View
    ID3D11Texture2D *pRenderTargetTexture;
    g_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pRenderTargetTexture);
    g_pD3D11Device->CreateRenderTargetView(pRenderTargetTexture, NULL, &g_pRenderTargetView);
    pRenderTargetTexture->Release();
    //Create Shader Resource View
    hr = CreateWICTextureFromFileEx(g_pD3D11Device, L"..\\Texture\\texture.png", 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
        0, WIC_LOADER_DEFAULT, NULL, &g_pShaderResourceView);
    if (FAILED(hr))
    {
        assert(0);
    }
    //Create Sampler State
    D3D11_SAMPLER_DESC sampleDesc;
    memset(&sampleDesc, 0, sizeof(sampleDesc));
    sampleDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    g_pD3D11Device->CreateSamplerState(&sampleDesc, &g_pSamplerState);
    //Create Depth Stencil View
    ID3D11Texture2D *pDSTexture = NULL;
    D3D11_TEXTURE2D_DESC descDSTex = { 0 };
    DXGI_SWAP_CHAIN_DESC descDXGI = { 0 };
    g_pDXGISwapChain->GetDesc(&descDXGI);
    descDSTex.ArraySize = 1;
    descDSTex.MipLevels = 1;
    descDSTex.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDSTex.Format = DXGI_FORMAT_D32_FLOAT;
    descDSTex.Usage = D3D11_USAGE_DEFAULT;
    descDSTex.SampleDesc.Count = 1;
    descDSTex.SampleDesc.Quality = 0;
    descDSTex.Width = descDXGI.BufferDesc.Width;
    descDSTex.Height = descDXGI.BufferDesc.Height;
    descDSTex.CPUAccessFlags = 0;
    descDSTex.MiscFlags = 0;
    g_pD3D11Device->CreateTexture2D(&descDSTex, NULL, &pDSTexture);
    g_pD3D11Device->CreateDepthStencilView(pDSTexture, NULL, &g_pDepthStencilView);
    pDSTexture->Release();
    //Create Rasterizer State
    D3D11_RASTERIZER_DESC descRast;
    memset(&descRast, 0, sizeof(descRast));
    descRast.CullMode = D3D11_CULL_BACK;
    descRast.FillMode = D3D11_FILL_SOLID;
    g_pD3D11Device->CreateRasterizerState(&descRast, &g_pRasterize);
	return hr;
}

HRESULT Render(HWND hwnd)
{
    HRESULT hr = S_OK;
    CreateD3D11Device(hwnd);
    InitD3D11Device();
    const UINT strideVB = sizeof(InputVertex), offsetVB = 0;
    g_pD3D11DeviceContext->IASetInputLayout(g_pD3D11InputLayout);
    g_pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    g_pD3D11DeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &strideVB, &offsetVB);
    g_pD3D11DeviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    g_pD3D11DeviceContext->VSSetShader(g_pVertexShader, NULL, 0);
    g_pD3D11DeviceContext->PSSetShader(g_pPixelShader, NULL, 0);
    const float pClearColor[] = {0.0f,0.5f,0.8f};
    g_pD3D11DeviceContext->ClearRenderTargetView(g_pRenderTargetView, pClearColor);
    g_pD3D11DeviceContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);
    g_pD3D11DeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
    g_pD3D11DeviceContext->PSSetSamplers(0, 1, &g_pSamplerState);
    g_pD3D11DeviceContext->PSSetShaderResources(0, 1, &g_pShaderResourceView);
    D3D11_VIEWPORT viewPort = { 0 };
    DXGI_SWAP_CHAIN_DESC descSwapChain = { 0 };
    g_pDXGISwapChain->GetDesc(&descSwapChain);
    viewPort.TopLeftX = (FLOAT)descSwapChain.BufferDesc.Width / 5;
    viewPort.TopLeftY = (FLOAT)descSwapChain.BufferDesc.Height / 5;
    viewPort.Width = (FLOAT)descSwapChain.BufferDesc.Width * 3 /5;
    viewPort.Height = (FLOAT)descSwapChain.BufferDesc.Height *3 /5;
    viewPort.MinDepth = 0.0;
    viewPort.MaxDepth = 1.0;
    g_pD3D11DeviceContext->RSSetViewports(1, &viewPort);
    g_pD3D11DeviceContext->RSSetState(g_pRasterize);
    g_pD3D11DeviceContext->DrawIndexed(6, 0, 0);

    g_pDXGISwapChain->Present(0, 0);
    return hr;
}

void CleanupDevice()
{
    if (g_pD3D11DeviceContext) g_pD3D11DeviceContext->ClearState();

    if (g_pDepthStencilView) g_pDepthStencilView->Release();
    if (g_pRenderTargetView) g_pRenderTargetView->Release();
    if (g_pSamplerState) g_pSamplerState->Release();
    if (g_pRasterize) g_pRasterize->Release();
    if (g_pShaderResourceView) g_pShaderResourceView->Release();
    if (g_pIndexBuffer) g_pIndexBuffer->Release();
    if (g_pVertexBuffer) g_pVertexBuffer->Release();
    if (g_pPixelShader) g_pPixelShader->Release();
    if (g_pVertexShader) g_pVertexShader->Release();
    if (g_pD3D11InputLayout) g_pD3D11InputLayout->Release();
    if (g_pDXGISwapChain) g_pDXGISwapChain->Release();
    if (g_pD3D11DeviceContext) g_pD3D11DeviceContext->Release();
    if (g_pD3D11Device) g_pD3D11Device->Release();
}