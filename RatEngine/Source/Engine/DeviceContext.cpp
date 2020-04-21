#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* context) : m_DeviceContext(context)
{
}

DeviceContext::~DeviceContext()
{
}

void DeviceContext::clearRenderTargetColor(SwapChain* swapChain, float r, float g, float b, float a)
{
	FLOAT clearColor[] = {r, g, b, a};
	m_DeviceContext->ClearRenderTargetView(swapChain->m_RenderTargetView, clearColor);
	m_DeviceContext->OMSetRenderTargets(1, &swapChain->m_RenderTargetView, NULL);
}

void DeviceContext::setVertexBuffer(VertexBuffer* vbuffer)
{
	UINT stride = vbuffer->m_VertexSize;
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, &vbuffer->m_VertexBuffer, &stride, &offset);
	m_DeviceContext->IASetInputLayout(vbuffer->m_InputLayout);
}

void DeviceContext::drawTriangleList(UINT vertexCount, UINT startVertexIndex)
{
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_DeviceContext->Draw(vertexCount, startVertexIndex);
}

void DeviceContext::drawTriangleStrip(UINT vertexCount, UINT startVertexIndex)
{
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	m_DeviceContext->Draw(vertexCount, startVertexIndex);
}

void DeviceContext::setViewportSize(FLOAT width, FLOAT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_DeviceContext->RSSetViewports(1, &vp);
}

bool DeviceContext::release()
{
	m_DeviceContext->Release();
	delete this;
	return true;
}
