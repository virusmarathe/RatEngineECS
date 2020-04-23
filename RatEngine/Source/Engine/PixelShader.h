#pragma once
#include <d3d11.h>

class GraphicsEngine;
class DeviceContext;

class PixelShader
{
public:
	PixelShader();
	~PixelShader();

	void release();
private:
	bool init(const void* shaderByteCode, SIZE_T byteCodeSize);

private:
	ID3D11PixelShader* m_PixelShader;

	friend class GraphicsEngine;
	friend class DeviceContext;
};

