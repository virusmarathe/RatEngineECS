#pragma once
#include <d3d11.h>

class DeviceContext;

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	// Initialize swapchain for a window
	bool init(HWND hwnd, UINT width, UINT height);

	bool present(bool vsync);

	// release swapchain resources
	bool release();

private:
	IDXGISwapChain* m_DXGISwapChain;
	ID3D11RenderTargetView* m_RenderTargetView;

	friend class DeviceContext;
};

