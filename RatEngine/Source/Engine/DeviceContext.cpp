#include "DeviceContext.h"
#include "SwapChain.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* context) : m_DeviceContext(context)
{
}

DeviceContext::~DeviceContext()
{
}

bool DeviceContext::clearRenderTargetColor(SwapChain* swapChain, float r, float g, float b, float a)
{
	FLOAT clearColor[] = {r, g, b, a};
	m_DeviceContext->ClearRenderTargetView(swapChain->m_RenderTargetView, clearColor);
	return true;
}

bool DeviceContext::release()
{
	m_DeviceContext->Release();
	delete this;
	return true;
}
