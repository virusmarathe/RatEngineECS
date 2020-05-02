#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	~SwapChain();

	bool present(bool vsync);

private:
	IDXGISwapChain* m_DXGISwapChain;
	ID3D11RenderTargetView* m_RenderTargetView;
	RenderSystem* m_RenderSystem;

	friend class DeviceContext;
};

