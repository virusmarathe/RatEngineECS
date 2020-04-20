#pragma once
#include <d3d11.h>

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	// Initialize swapchain for a window
	bool init(HWND hwnd, UINT width, UINT height);

	// release swapchain resources
	bool release();

private:
	IDXGISwapChain* m_DXGISwapChain;
};

