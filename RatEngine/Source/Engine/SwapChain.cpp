#include "SwapChain.h"
#include "GraphicsEngine.h"

SwapChain::SwapChain() : m_DXGISwapChain(NULL), m_RenderTargetView(NULL)
{
}

SwapChain::~SwapChain()
{
}

bool SwapChain::init(HWND hwnd, UINT width, UINT height)
{
	ID3D11Device* device = GraphicsEngine::get()->m_D3DDevice;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;

	// create the swap chain for the window indicated by the HWND parameter
	HRESULT result = GraphicsEngine::get()->m_DXGIFactory->CreateSwapChain(device, &desc, &m_DXGISwapChain);

	if (FAILED(result))
	{
		return false;
	}

	ID3D11Texture2D* buffer;
	result = m_DXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(result))
	{
		return false;
	}

	result = device->CreateRenderTargetView(buffer, NULL, &m_RenderTargetView);
	buffer->Release();

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool SwapChain::present(bool vsync)
{
	m_DXGISwapChain->Present(vsync, NULL);
	return true;
}

bool SwapChain::release()
{
	m_DXGISwapChain->Release();
	delete this;
	return true;
}
