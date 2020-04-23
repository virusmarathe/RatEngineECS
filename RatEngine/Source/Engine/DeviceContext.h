#pragma once
#include <d3d11.h>

class SwapChain;
class VertexBuffer;
class VertexShader;
class PixelShader;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* context);
	~DeviceContext();

	void clearRenderTargetColor(SwapChain* swapChain, float r, float g, float b, float a);
	void setVertexBuffer(VertexBuffer* vbuffer);
	void drawTriangleList(UINT vertexCount, UINT startVertexIndex);
	void drawTriangleStrip(UINT vertexCount, UINT startVertexIndex);
	void setViewportSize(FLOAT width, FLOAT height);

	void setVertexShader(VertexShader* vs);
	void setPixelShader(PixelShader* ps);

	bool release();

private:
	ID3D11DeviceContext* m_DeviceContext;
};

