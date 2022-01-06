#include "stdafx.h"
#include <d3d11.h>
#include <dxgi.h>
#include <xnamath.h>


//Global Varibles
ID3D11Device     *g_pD3D11Device;
IDXGISwapChain   *g_pDXGISwapChain;
ID3D11DeviceContext *g_pD3D11DeviceContext;
ID3D11InputLayout   *g_pD3D11InputLayout;
ID3D11Buffer        *g_pVertexBuffer;
ID3D11Buffer        *g_pIndexBuffer;
ID3D11RenderTargetView *g_pRenderTargetView;
ID3D11DepthStencilView *g_pDepthStencilView;

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
    swapChainDesc.Windowed = TRUE;

    for (DWORD i = 0; i < driverTypeNum; i++)
    {
        hr = D3D11CreateDeviceAndSwapChain(NULL, pDriverType[i], NULL, D3D11_CREATE_DEVICE_DEBUG, pD3DFeatureLevel, ARRAYSIZE(pD3DFeatureLevel), D3D11_SDK_VERSION, &swapChainDesc,
            &g_pDXGISwapChain, &g_pD3D11Device, pD3DFeatureLevel, &g_pD3D11DeviceContext);
        if(SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
        return hr;
}

HRESULT InitD3D11Device()
{

}