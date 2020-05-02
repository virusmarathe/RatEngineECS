#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class PixelShader
{
public:
	PixelShader(const void* shaderByteCode, SIZE_T byteCodeSize, RenderSystem* system);
	~PixelShader();

private:
	ID3D11PixelShader* m_PixelShader;
	RenderSystem* m_RenderSystem;

	friend class RenderSystem;
	friend class DeviceContext;
};

