#pragma once
#include <d3d11.h>

class GraphicsEngine;
class DeviceContext;

class VertexShader
{
public:
	VertexShader();
	~VertexShader();
	void release();

private:
	bool init(const void* shaderByteCode, SIZE_T byteCodeSize);

private:
	ID3D11VertexShader* m_Shader;

	friend class GraphicsEngine;
	friend class DeviceContext;
};

