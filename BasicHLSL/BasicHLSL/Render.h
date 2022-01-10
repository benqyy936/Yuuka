#pragma once
#include <DirectXMath.h>
using namespace DirectX;
typedef struct _InputVertex
{
    XMFLOAT3 position;
    XMFLOAT4 color;
}InputVertex; 

HRESULT CreateD3D11Device(HWND hWnd);
HRESULT InitD3D11Device();
HRESULT Render(HWND hwnd);
void CleanupDevice();
