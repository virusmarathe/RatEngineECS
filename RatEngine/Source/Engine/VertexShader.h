#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class VertexShader
{
public:
	VertexShader(const void* shaderByteCode, SIZE_T byteCodeSize, RenderSystem* system);
	~VertexShader();

private:
	ID3D11VertexShader* m_Shader;
	RenderSystem* m_RenderSystem;

	friend class RenderSystem;
	friend class DeviceContext;
};

