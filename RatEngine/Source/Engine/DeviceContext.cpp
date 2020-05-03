#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include <exception>

DeviceContext::DeviceContext(ID3D11DeviceContext* context, RenderSystem* system) : m_DeviceContext(context), m_RenderSystem(system)
{
}

DeviceContext::~DeviceContext()
{
	m_DeviceContext->Release();
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

void DeviceContext::drawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation)
{
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->DrawIndexed(indexCount, startIndexLocation, startVertexIndex);
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

void DeviceContext::setVertexShader(VertexShader* vs)
{
	m_DeviceContext->VSSetShader(vs->m_Shader, nullptr, 0);
}

void DeviceContext::setPixelShader(PixelShader* ps)
{
	m_DeviceContext->PSSetShader(ps->m_PixelShader, nullptr, 0);
}

void DeviceContext::setTexture(VertexShader* vs, const TexturePointer& texture)
{
	m_DeviceContext->VSSetShaderResources(0, 1, &texture->m_ShaderResourceView);
}

void DeviceContext::setTexture(PixelShader* ps, const TexturePointer& texture)
{
	m_DeviceContext->PSSetShaderResources(0, 1, &texture->m_ShaderResourceView);
}

void DeviceContext::setConstantBuffer(VertexShader* vs, ConstantBuffer* constantBuf)
{
	m_DeviceContext->VSSetConstantBuffers(0, 1, &constantBuf->m_Buffer);
}

void DeviceContext::setConstantBuffer(PixelShader* ps, ConstantBuffer* constantBuf)
{
	m_DeviceContext->PSSetConstantBuffers(0, 1, &constantBuf->m_Buffer);
}

void DeviceContext::setIndexBuffer(IndexBuffer* iBuffer)
{
	m_DeviceContext->IASetIndexBuffer(iBuffer->m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}