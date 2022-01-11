#pragma once
#include <DirectXMath.h>
using namespace DirectX;
typedef struct _InputVertex
{
    XMFLOAT3 position;
    XMFLOAT2 texcoord;
}InputVertex; 

HRESULT CreateD3D11Device(HWND hWnd);
HRESULT InitD3D11Device();
HRESULT Render(HWND hwnd);
void CleanupDevice();
