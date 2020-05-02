#include "SwapChain.h"
#include "RenderSystem.h"
#include <exception>

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system) : 
	m_DXGISwapChain(NULL), m_RenderTargetView(NULL), m_RenderSystem(system)
{
	ID3D11Device* device = m_RenderSystem->m_D3DDevice;

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
	HRESULT result = m_RenderSystem->m_DXGIFactory->CreateSwapChain(device, &desc, &m_DXGISwapChain);

	if (FAILED(result))	throw std::exception("SwapChain not created successfully");

	ID3D11Texture2D* buffer;
	result = m_DXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(result))	throw std::exception("SwapChain::GetBuffer not created successfully");

	result = device->CreateRenderTargetView(buffer, NULL, &m_RenderTargetView);
	buffer->Release();

	if (FAILED(result))	throw std::exception("SwapChain::CreateRenderTargetView not created successfully");
}

SwapChain::~SwapChain()
{
	if (m_RenderTargetView) m_RenderTargetView->Release();
	if (m_DXGISwapChain) m_DXGISwapChain->Release();
}

bool SwapChain::present(bool vsync)
{
	m_DXGISwapChain->Present(vsync, NULL);
	return true;
}