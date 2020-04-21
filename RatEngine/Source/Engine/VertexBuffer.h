#pragma once
#include <d3d11.h>

class DeviceContext;

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	bool load(void* listVertices, UINT sizeVertex, UINT numVertices, void* shaderByteCode, UINT shaderByteSize);
	bool release();

	UINT getNumVertices() { return m_NumVertices; }

private:
	UINT m_VertexSize;
	UINT m_NumVertices;

	ID3D11Buffer* m_VertexBuffer;
	ID3D11InputLayout* m_InputLayout;

	friend class DeviceContext;
};

