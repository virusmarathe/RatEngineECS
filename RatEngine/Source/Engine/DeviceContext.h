#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* context, RenderSystem* system);
	~DeviceContext();

	void clearRenderTargetColor(SwapChain* swapChain, float r, float g, float b, float a);
	void setVertexBuffer(VertexBuffer* vbuffer);
	void drawTriangleList(UINT vertexCount, UINT startVertexIndex);
	void drawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation);
	void drawTriangleStrip(UINT vertexCount, UINT startVertexIndex);
	void setViewportSize(FLOAT width, FLOAT height);

	void setVertexShader(VertexShader* vs);
	void setPixelShader(PixelShader* ps);

	void setTexture(VertexShader* vs, const TexturePointer& texture);
	void setTexture(PixelShader* ps, const TexturePointer& texture);

	void setConstantBuffer(VertexShader* vs, ConstantBuffer * constantBuf);
	void setConstantBuffer(PixelShader* ps, ConstantBuffer * constantBuf);

	void setIndexBuffer(IndexBuffer* iBuffer);

private:
	ID3D11DeviceContext* m_DeviceContext;
	RenderSystem* m_RenderSystem;

	friend class ConstantBuffer;
	friend class IndexBuffer;
};

